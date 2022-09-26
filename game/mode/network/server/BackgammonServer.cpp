#include <iostream>
#include "BackgammonServer.h"

/* private */
Room::Room(const std::string& password) {
    this->password = password;
}
/* public */


/* private */
void BackgammonServer::on_event(Client_t sender, const std::string& event) {
    this->on_event(sender, event::parse_message(event));
}
void BackgammonServer::on_event(Client_t sender, event::Event* event) {
    if (event->id == Event_t::CREATE_ROOM || event->id == Event_t::CREATE_ROOM_AND_CONNECT) {
        for (auto & room : this->rooms){
            if (room.first == event->room) {
                this->serverSocket->send_to(sender, event::RoomExists(event->room, event->password).to_string());
                return;
            }
        }
        this->rooms.insert({event->room, new Room(event->password)});
    }

    Room* room = this->rooms[event->room];
    if (event->password != room->password) {
        this->serverSocket->send_to(sender, event::IncorrectPassword(event->room, event->password).to_string());
        return;
    }

    Client_t* sender_opponent = nullptr;
    if (room->white_player && sender.socket_fd == room->white_player->socket_fd) {
        sender_opponent = room->black_player;
    } else if (room->black_player && sender.socket_fd == room->black_player->socket_fd) {
        sender_opponent = room->white_player;
    }

    switch (event->id) {
        case Event_t::CREATE_ROOM_AND_CONNECT:
        case Event_t::CONNECT_ROOM: {
            if (room->white_player == nullptr) {
                room->white_player = &sender;
                this->serverSocket->send_to(sender, event::SetMyself(event->room, "", Player_t::WHITE).to_string());
            } else if (room->black_player == nullptr) {
                room->black_player = &sender;
                this->serverSocket->send_to(sender, event::SetMyself(event->room, "", Player_t::BLACK).to_string());
            }

            if (room->white_player && room->black_player) {
                /* todo choose player */
                this->serverSocket->send_to(*room->white_player,
                    event::Start("", "", Player_t::WHITE).to_string()
                );
                this->serverSocket->send_to(*room->black_player,
                    event::Start("", "", Player_t::WHITE).to_string()
                );
            }
            break;
        }
        case Event_t::GAME_OVER: {
            this->serverSocket->send_to(*sender_opponent, event->to_string());
            break;
        }
        case Event_t::THROW_DICES: {
            this->serverSocket->send_to(*sender_opponent, event->to_string());
            break;
        }
        case Event_t::TAKE_PEACE: {
            this->serverSocket->send_to(*sender_opponent, event->to_string());
            break;
        }
        case Event_t::MOVE_TO: {
            this->serverSocket->send_to(*sender_opponent, event->to_string());
            break;
        }
        case Event_t::CANCEL_MOVES: {
            this->serverSocket->send_to(*sender_opponent, event->to_string());
            break;
        }
        case Event_t::RELEASE_PEACE: {
            this->serverSocket->send_to(*sender_opponent, event->to_string());
            break;
        }
        case Event_t::COMMIT_MOVES: {
            this->serverSocket->send_to(*sender_opponent, event->to_string());
            break;
        }
        default:
            break;
    }
}
/* public */
BackgammonServer::BackgammonServer() {
    this->serverSocket = new Socket::Server();
    this->serverSocket->on_connect = [&](Client_t client) {
        this->serverSocket->wait_message_from(client, 200).detach();
    };
    this->serverSocket->on_disconnect = [&](Client_t client) {
        /* find disconnected player room and remove */
        for (auto it = this->rooms.begin(); it != this->rooms.end(); ++it){
            if (it->second->white_player->socket_fd == client.socket_fd ||
                it->second->black_player->socket_fd == client.socket_fd) {

                this->rooms.erase(it);
                this->serverSocket->close(*it->second->white_player);
                this->serverSocket->close(*it->second->black_player);
                break;
            }
        }
    };

    this->serverSocket->on_message_from = [&](const std::string &data, Client_t from) {
        this->on_event(from, data);
    };
}

std::thread BackgammonServer::run(uint16_t port, uint64_t max_rooms) {
    this->serverSocket->max_connections = max_rooms * 2;
    this->serverSocket->create();
    this->serverSocket->bind(port);
    this->serverSocket->listen(max_rooms * 2); /* on every room can set_server to player */

    return this->serverSocket->wait_connection();
}

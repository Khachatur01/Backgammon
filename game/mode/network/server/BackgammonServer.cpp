#include <iostream>
#include "BackgammonServer.h"

/* private */
Room::Room(const std::string& password) {
    this->password = password;
}
/* public */


/* private */
void BackgammonServer::on_event(Client sender, const std::string& event) {
    this->on_event(sender, event::parse_message(event));
}
void BackgammonServer::on_event(Client sender, event::Event* event) {
    if (event->id == Event_t::CREATE_ROOM) {
        this->rooms.insert({event->room, new Room(event->password)});
        std::cout << "\nCreated room '" << event->room << "'\n";
        return;
    } else if (event->id == Event_t::CREATE_ROOM_AND_CONNECT) {
        this->rooms.insert({event->room, new Room(event->password)});
        std::cout << "\nCreated room and connect '" << event->room << "'\n";

        Room* room = this->rooms[event->room];
        /* use Player_t::SWITCH for server */
        if (room->white_player == nullptr) {
            room->white_player = &sender;
            this->serverSocket->send_to(sender, event::SetMyself("", "", Player_t::WHITE).to_string());
        } else if (room->black_player == nullptr) {
            room->black_player = &sender;
            this->serverSocket->send_to(sender, event::SetMyself("", "", Player_t::BLACK).to_string());
        }

        if (room->white_player && room->black_player) {
            std::cout << "Start game '" << event->room << "'\n";
            this->serverSocket->send_to(*room->white_player,
                event::Start("", "", Player_t::WHITE).to_string()
            );
            this->serverSocket->send_to(*room->black_player,
                event::Start("", "", Player_t::WHITE).to_string()
            );
        }
        return;
    }

    Room* room = this->rooms[event->room];
    if (event->password != room->password) {
        std::cout << "\nIncorrect password\n";
        return;
    }

    Client* sender_opponent = nullptr;
    if (room->white_player && sender.socket_fd == room->white_player->socket_fd) {
        sender_opponent = room->black_player;
    } else if (room->black_player && sender.socket_fd == room->black_player->socket_fd) {
        sender_opponent = room->white_player;
    }

    switch (event->id) {
        case Event_t::CONNECT_ROOM: {
            std::cout << "\nConnected room '" << event->room << "'\n";
            /* use Player_t::SWITCH for server */
            if (room->white_player == nullptr) {
                room->white_player = &sender;
                this->serverSocket->send_to(sender, event::SetMyself("", "", Player_t::WHITE).to_string());
            } else if (room->black_player == nullptr) {
                room->black_player = &sender;
                this->serverSocket->send_to(sender, event::SetMyself("", "", Player_t::BLACK).to_string());
            }

            if (room->white_player && room->black_player) {
                std::cout << "Start game '" << event->room << "'\n";
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
        case Event_t::THROW_DICES: {
            auto* throw_dices = dynamic_cast<event::ThrowDices*>(event);
            this->serverSocket->send_to(*sender_opponent, event::ThrowDices("", "", throw_dices->first, throw_dices->second).to_string());
            break;
        }
        case Event_t::TAKE_PEACE: {
            auto* take_peaces = dynamic_cast<event::TakePeace*>(event);
            this->serverSocket->send_to(*sender_opponent, event::TakePeace("", "", take_peaces->pip).to_string());
            break;
        }
        case Event_t::MOVE_TO: {
            auto* move_to = dynamic_cast<event::MoveTo*>(event);
            this->serverSocket->send_to(*sender_opponent, event::MoveTo("", "", move_to->pip).to_string());
            break;
        }
        case Event_t::CANCEL_MOVES: {
            this->serverSocket->send_to(*sender_opponent, event::CancelMoves("", "").to_string());
            break;
        }
        case Event_t::RELEASE_PEACE: {
            this->serverSocket->send_to(*sender_opponent, event::ReleasePeace("", "").to_string());
            break;
        }
        case Event_t::COMMIT_MOVES: {
            this->serverSocket->send_to(*sender_opponent, event::CommitMoves("", "").to_string());
            break;
        }
        default:
            break;
    }
}
/* public */
BackgammonServer::BackgammonServer() {
    this->serverSocket = new ServerSocket();
    this->serverSocket->on_connect = [&](Client client) {
        this->serverSocket->wait_message_from(client, 200).detach();
    };
    this->serverSocket->on_disconnect = [&](Client client) {
        /* find disconnected player room and remove */
        for (auto it = this->rooms.begin(); it != this->rooms.end(); ++it){
            if (it->second->white_player->socket_fd == client.socket_fd ||
                it->second->black_player->socket_fd == client.socket_fd ) {

                this->rooms.erase(it);
                break;
            }
        }
    };

    this->serverSocket->on_message_from = [&](const std::string &data, Client from) {
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

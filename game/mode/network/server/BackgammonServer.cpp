#include <random>
#include "BackgammonServer.h"

/* private */
Room::Room(const std::string& password) {
    this->password = password;
}
/* public */


/* private */
void BackgammonServer::on_message(Client_t sender, const std::string& message) {
    this->on_message(sender, event::parse_message(message));
}
void BackgammonServer::on_message(Client_t sender, event::Event* event) {
    if (event->id == Event_t::CREATE_ROOM || event->id == Event_t::CREATE_ROOM_AND_CONNECT) {
        for (auto& room : this->rooms){
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
                std::random_device random_device;
                std::default_random_engine engine(random_device());
                std::uniform_int_distribution<uint8_t> uniform_dist(0, 1);

                Player_t starter;
                if (uniform_dist(engine) == 0) {
                    starter = Player_t::WHITE;
                } else {
                    starter = Player_t::BLACK;
                }

                this->serverSocket->send_to(*room->white_player,
                    event::Start("", "", starter).to_string()
                );
                this->serverSocket->send_to(*room->black_player,
                    event::Start("", "", starter).to_string()
                );
            }
            break;
        }

        case Event_t::GAME_OVER:
        case Event_t::THROW_DICES:
        case Event_t::TAKE_PEACE:
        case Event_t::MOVE_TO:
        case Event_t::CANCEL_MOVES:
        case Event_t::RELEASE_PEACE:
        case Event_t::COMMIT_MOVES:
            this->serverSocket->send_to(*sender_opponent, event->to_string());
            break;
        default:
            break;
    }
    delete event;
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
        this->on_message(from, data);
    };
}
BackgammonServer::~BackgammonServer() {
    delete this->serverSocket;
    for (auto &room : this->rooms) {
        delete room.second;
    }
}

std::thread BackgammonServer::run(uint16_t port, uint64_t max_rooms) {
    this->serverSocket->max_connections = max_rooms * 2;
    this->serverSocket->create();
    this->serverSocket->bind(port);
    this->serverSocket->listen(max_rooms * 2); /* on every room can set_server to player */

    return this->serverSocket->wait_connection();
}

#include <iostream>
#include <utility>
#include "BackgammonServer.h"

/* private */
Room::Room(const std::string& password) {
    this->password = password;
}
/* public */


/* private */
/* public */
BackgammonServer::BackgammonServer() : serverSocket(1) {
    this->serverSocket.on_connect = [&](Client client) {
        this->serverSocket.wait_message_from(client, 200).detach();
    };
    this->serverSocket.on_disconnect = [&](Client client) {
        std::cout << client.socket_fd << " is disconnected\n";
    };

    this->serverSocket.on_message_from = [&](const std::string &data, Client from) {
        this->on_event(data);
    };
}

std::thread BackgammonServer::run(uint16_t port, uint64_t max_rooms) {
    this->serverSocket.create();
    this->serverSocket.bind(port);
    this->serverSocket.listen(max_rooms * 2); /* on every room can connect to player */

    return this->serverSocket.wait_connection();

}

void BackgammonServer::on_event(const std::string& event) {
    this->on_event(event::parse(event));
}
void BackgammonServer::on_event(event* event) {
    Room room = this->rooms.at(event->room);
    if (event->password != room.password) {
        return;
    }

    switch (event->id) {
        case CREATE_ROOM: {
            break;
        }
        case CONNECT_ROOM: {
            break;
        }
        case THROW_DICES: {
            auto* throw_dices_event = dynamic_cast<throw_dices*>(event);
            dices_t force_dices = std::make_pair(throw_dices_event->first, throw_dices_event->first);
            room.backgammon.throw_dice(true, &force_dices);
            break;
        }
        case TAKE_PEACE: {
            auto* take_peace_event = dynamic_cast<take_peace*>(event);
            room.backgammon.take_peace(take_peace_event->pip, true);
            break;
        }
        case MOVE_TO: {
            auto* move_to_event = dynamic_cast<move_to*>(event);
            room.backgammon.take_peace(move_to_event->pip, true);
            break;
        }
        case CANCEL_MOVES: {
            room.backgammon.cancel_moves();
            break;
        }
        case RELEASE_PEACE: {
            room.backgammon.release_peace();
            break;
        }
        case COMMIT_MOVES: {
            room.backgammon.commit_moves();
            break;
        }
        default:
            break;
    }
}
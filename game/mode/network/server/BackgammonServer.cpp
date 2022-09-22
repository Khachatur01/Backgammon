#include <iostream>
#include "BackgammonServer.h"

/* private */

/* public */
BackgammonServer::BackgammonServer() : serverSocket(1), eventHandler(false, Player_t::WHITE) {
    this->serverSocket.on_connect = [&](Client client) {
        this->players_map[Player_t::BLACK] = client; /* second player is black */
    };
    this->serverSocket.on_disconnect = [&](Client client) {
        std::cout << client.socket_fd << " is disconnected\n";
    };

    this->serverSocket.on_message_from = [&](const std::string &data, Client from) {
        this->eventHandler.handle(data);
    };
}

void BackgammonServer::run(uint16_t port) {
    this->serverSocket.create();
    this->serverSocket.bind(port);
    this->serverSocket.listen(1);

    this->serverSocket.wait_connection().join(); /* wait for second player to connect */

    /* start game when second player is connected */
    this->eventHandler.get_backgammon()->start();

    std::string enter;
    switch (this->eventHandler.get_backgammon()->get_player()->TYPE) {
        case Player_t::WHITE: {
            std::cout << "Input something to throw dices: ";
            std::cin >> enter;
            dices_t dices = this->eventHandler.get_backgammon()->throw_dice();
            this->serverSocket.send_to(this->players_map[Player_t::BLACK], throw_dices(dices.first, dices.second).to_string());
            break;
        }
        case Player_t::BLACK: {
            std::cout << "Opponent will start...\n ";
            this->serverSocket.send_to(this->players_map[Player_t::BLACK], start(Player_t::BLACK).to_string());
            break;
        }
        default:
            break;
    }

    this->serverSocket.wait_message_from(this->players_map[Player_t::BLACK], 200).join();
}

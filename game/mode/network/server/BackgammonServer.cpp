#include <iostream>
#include "BackgammonServer.h"

/* private */

/* public */
BackgammonServer::BackgammonServer() : serverSocket(1) {
    this->serverSocket.on_connect = [&](Client client) {
        this->players_map[Player_t::BLACK] = client; /* second player is black */
    };
    this->serverSocket.on_disconnect = [&](Client client) {
        std::cout << client.socket_fd << " is disconnected\n";
    };

    this->serverSocket.on_message_from = [&](const std::string &data, Client from) {
        if (data == "throw") {
            dices_t force_dices = std::make_pair(5, 6);
            this->backgammon.throw_dice(true, &force_dices);
        }
    };
}

void BackgammonServer::run(uint16_t port) {
    this->serverSocket.create();
    this->serverSocket.bind(port);
    this->serverSocket.listen(1);

    this->serverSocket.wait_connection().join(); /* wait for second player to connect */

    /* when second player is connected, start game */
    this->backgammon.auto_commit = false;
    this->backgammon.viewer = Player_t::WHITE;
    this->backgammon.start();

    std::string enter;
    switch (this->backgammon.get_player()->TYPE) {
        case Player_t::WHITE:
            std::cout << "Input something to throw dices: ";
            std::cin >> enter;
            this->backgammon.throw_dice();
            this->serverSocket.send_to(this->players_map[Player_t::BLACK], "throw");
            break;
        case Player_t::BLACK:
            std::cout << "Opponent will start...\n ";
            this->serverSocket.send_to(this->players_map[Player_t::BLACK], "start");
            break;
        default:
            break;
    }

    this->serverSocket.wait_message_from(this->players_map[Player_t::BLACK], 200).join();
}

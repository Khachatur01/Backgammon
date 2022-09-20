#include <iostream>
#include "BackgammonClient.h"
/* private */
/* public */
BackgammonClient::BackgammonClient() {
    this->clientSocket.on_message = [&](const std::string &data) {
        if (data == "start") {
            this->backgammon.set_player(Player_t::BLACK);
            this->backgammon.throw_dice();
        } else if (data == "throw") {
            dices_t force_dices = std::make_pair(5, 6);
            this->backgammon.throw_dice(false, &force_dices);
            this->backgammon.render(Player_t::BLACK);
        }
    };
    this->clientSocket.on_disconnect = []() {

    };

    this->clientSocket.create();
}

void BackgammonClient::connect(const std::basic_string<char> &hostname, uint16_t port) {
    this->backgammon.auto_commit = false;
    this->backgammon.viewer = Player_t::BLACK;
    this->clientSocket.init_host(hostname, port);
    this->clientSocket.connect();

    std::cout << "connected\n";

    this->clientSocket.wait_message(200).join();
}
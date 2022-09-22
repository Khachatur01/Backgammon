#include <iostream>
#include "BackgammonClient.h"
/* private */
/* public */
BackgammonClient::BackgammonClient(): eventHandler(false, Player_t::BLACK) {
    this->clientSocket.on_message = [&](const std::string &data) {
        std::cout << data << std::endl;
        this->eventHandler.handle(data);
    };
    this->clientSocket.on_disconnect = []() {

    };

    this->clientSocket.create();
}

void BackgammonClient::connect(const std::basic_string<char> &hostname, uint16_t port) {
    this->clientSocket.init_host(hostname, port);
    this->clientSocket.connect();

    std::cout << "connected\n";

    this->clientSocket.wait_message(200).join();
}
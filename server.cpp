#include <iostream>
#include "game/mode/network/server/BackgammonServer.h"

int main() {
    uint16_t port;
    std::cout << "Welcome to server creator for backgammon game\n";
    std::cout << "Run server on port: ";
    std::cin >> port;

    BackgammonServer backgammonServer;
    backgammonServer.run(port, 10).detach();

    char c;
    std::cout << "Input anything to stop server: ";
    std::cin >> c;
}
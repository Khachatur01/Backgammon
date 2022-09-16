#ifndef IO_STREAM
#include <iostream>
#define IO_STREAM
#endif

#include "game/mode/local.cpp"
#include "game/mode/network.cpp"
#include "game/mode/bot.cpp"

int main() {
    char game_mode;
    std::cout << "Welcome to Backgammon console game.\n";
    std::cout << "Select game mode.\n";
    std::cout << "a) Local 2 player\n";
    std::cout << "b) Network 2 player\n";
    std::cout << "c) Play with computer\n";

    do {
        std::cin >> game_mode;
    } while (game_mode < 'a' || game_mode > 'c');

    switch (game_mode) {
        case 'a':
            local_mode();
            break;
        case 'b':
            network_mode();
            break;
        case 'c':
            bot_mode();
            break;
        default:
            break;
    }
}
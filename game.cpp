#include <iostream>
#include "game/mode/local.cpp"
#include "game/mode/network/network.cpp"
#include "game/mode/bot.cpp"

int main() {
    uint8_t game_mode;
    do {
        Backgammon::clear_screen();
        std::cout << "Welcome to Backgammon console game.\n";
        std::cout << "Select game mode.\n";
        std::cout << "<) Exit\n";
        std::cout << "a) Local 2 player\n";
        std::cout << "b) Network 2 player\n";
        std::cout << "c) Play with computer\n";

        do {
            std::cin >> game_mode;
        } while (game_mode != '<' && game_mode < 'a' || game_mode > 'b');

        if (game_mode == '<') {
            break;
        }

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
    } while(true);

    return 0;
}
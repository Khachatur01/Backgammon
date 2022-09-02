#include <iostream>
#include "game/backgammon/backgammon.h"

int main() {
    Backgammon backgammon;

    backgammon.start();
    std::string enter;
    std::cout << "Press Enter to throw dices: ";
    getline(std::cin, enter);
    backgammon.throw_dice();

    game_over_checking_loop:
    while (!backgammon.is_game_over()) {
        uint8_t pip_index;
        uint8_t to;

        std::cout << "Take peace(0 to cancel all moves. 1 to commit): ";
        std::cin >> pip_index;
        if (pip_index == '0') {
            backgammon.cancel_moves();
            continue;
        } else if (pip_index == '1' && backgammon.commit_moves()) {
            backgammon.throw_dice();
            goto game_over_checking_loop;
        }
        while (!backgammon.take_peace(pip_index)) {
            std::cout << "Invalid peace! Take another(0 to cancel all moves. 1 to commit): ";
            std::cin >> pip_index;
            if (pip_index == '0') {
                backgammon.cancel_moves();
                continue;
            } else if (pip_index == '1' && backgammon.commit_moves()) {
                backgammon.throw_dice();
                goto game_over_checking_loop;
            }
        }

        std::cout << "Move to(0 to cancel all moves): ";
        std::cin >> to;
        if (to == '0') {
            backgammon.cancel_moves();
            continue;
        }
        while (!backgammon.move_to(to)) {
            std::cout << "Invalid move! Take another(0 to cancel all moves): ";
            std::cin >> to;
            if (to == '0') {
                backgammon.cancel_moves();
                continue;
            }
        }
    }
    return 0;
}

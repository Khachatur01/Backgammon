#include <iostream>
#include "game/backgammon/backgammon.h"

int main() {
    Backgammon backgammon;

    backgammon.start();
    backgammon.auto_commit = false;
    std::string enter;
    std::cout << "Press Enter to throw dices: ";
    getline(std::cin, enter);
    backgammon.throw_dice();

    game_over_checking_loop:
    while (backgammon.get_winner() == nullptr) {
        uint8_t pip_index;
        uint8_t to;

        do {
            backgammon.render(); /* to clear unnecessary messages */
            std::cout << "Take peace(0 to cancel all moves. 1 to commit): ";
            std::cin >> pip_index;
            if (pip_index == '0') {
                backgammon.cancel_moves();
                continue;
            } else if (pip_index == '1' && backgammon.commit_moves()) {
                backgammon.throw_dice();
                goto game_over_checking_loop;
            }
        } while (!backgammon.take_peace(pip_index));

        do {
            backgammon.render(); /* to clear unnecessary messages */
            std::cout << "Move to(0 to cancel all moves): ";
            std::cin >> to;
            if (to == '0') {
                backgammon.cancel_moves();
                continue;
            }
        } while (!backgammon.move_to(to));
    }
    std::cout << "Game over\n" << "Winner is " << backgammon.get_winner()->PEACE;
    return 0;
}
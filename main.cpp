#include <iostream>
#include "game/backgammon/backgammon.h"

int main() {
    Backgammon backgammon;

    backgammon.start();
    backgammon.auto_commit = true;
    std::string enter;
    std::cout << "Press Enter to throw dices: ";
    getline(std::cin, enter);
    backgammon.throw_dice();

    std::string test = backgammon.get_frame();
    game_over_checking_loop:
    while (backgammon.get_winner() == nullptr) {
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
            backgammon.render();
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
            backgammon.render();
            std::cout << "Invalid move!(0 to cancel selection): ";
            std::cin >> to;
            if (to == '0') {
                backgammon.release_peace();
                goto game_over_checking_loop;
            }
        }
    }
    std::cout << "Game over\n" << "Winner is " << backgammon.get_winner()->PEACE;
    return 0;
}

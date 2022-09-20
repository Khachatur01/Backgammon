#include <iostream>
#include "../backgammon/backgammon.h"

void local_mode() {
    std::string enter;
    do {
        Backgammon::clear_screen();
        Backgammon backgammon;
        backgammon.start();
        backgammon.auto_commit = false;
        std::cout << "Input something to throw dices: ";
        std::cin >> enter;
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
                    if (backgammon.get_winner() == nullptr) {
                        backgammon.throw_dice(); /* if the game is not over, throw dices */
                    }
                    goto game_over_checking_loop;
                }
            } while (!backgammon.take_peace(pip_index));

            do {
                backgammon.render(); /* to clear unnecessary messages */
                std::cout << "Move to(0 to release peace): ";
                std::cin >> to;
                if (to == '0') {
                    backgammon.release_peace();
                    goto game_over_checking_loop;
                }
            } while (!backgammon.move_to(to));
        }
        std::cout << "Game over\n" << "Winner is " << backgammon.get_winner()->PEACE << '\n';
        std::cout << "    <) Back\n";
        std::cout << "Enter) Play again\n";
        std::cin >> enter;
        if (enter == "<") {
            break;
        }
    } while (true);
}
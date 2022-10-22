#include <iostream>
#include "../backgammon/backgammon.h"

void local_mode() {
    std::string enter;
    do {
        Backgammon::clear_screen();
        Backgammon backgammon(false, Player_t::SWITCH);
        backgammon.start();
        std::cout << "Input something to throw dices: ";
        std::cin >> enter;
        backgammon.throw_dice();

        game_over_checking_loop:
        while (backgammon.get_winner() == nullptr) {
            uint8_t pip;
            uint8_t to;

            do {
                backgammon.render(); /* to clear unnecessary messages */
                std::cout << "Take peace(0 to cancel all moves. 1 to commit): ";
                std::cin >> pip;
                if (pip == '0') {
                    backgammon.cancel_moves();
                    continue;
                } else if (pip == '1' && backgammon.commit_moves()) {
                    if (backgammon.get_winner() == nullptr) {
                        backgammon.throw_dice(); /* if the game is not over, throw dices */
                    }
                    goto game_over_checking_loop;
                }
            } while (!backgammon.take_peace(pip));

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
        Backgammon::clear_screen();
        std::cout << "\n"
                     " ██████╗  █████╗ ███╗   ███╗███████╗       █████╗ ██╗   ██╗███████╗██████╗ \n"
                     "██╔════╝ ██╔══██╗████╗ ████║██╔════╝      ██╔══██╗██║   ██║██╔════╝██╔══██╗\n"
                     "██║  ██╗ ███████║██╔████╔██║█████╗        ██║  ██║╚██╗ ██╔╝█████╗  ██████╔╝\n"
                     "██║  ╚██╗██╔══██║██║╚██╔╝██║██╔══╝        ██║  ██║ ╚████╔╝ ██╔══╝  ██╔══██╗\n"
                     "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗      ╚█████╔╝  ╚██╔╝  ███████╗██║  ██║\n"
                     " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝       ╚════╝    ╚═╝   ╚══════╝╚═╝  ╚═╝\n"
                     << "Winner is " << backgammon.get_winner()->PEACE << '\n';
        std::cout << "    <) Back\n";
        std::cout << "Enter) Play again\n";
        std::cin >> enter;
        if (enter == "<") {
            break;
        }
    } while (true);
}
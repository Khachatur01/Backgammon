#include <iostream>
#include "game/backgammon.h"

int main() {
    Backgammon backgammon;
    while (!backgammon.is_game_over()) {
        std::string enter;
        std::cout << "Press Enter to throw dices: ";
        getline(std::cin, enter);
        backgammon.throw_dice();

        for (uint8_t move = 0; move < backgammon.get_available_moves_count(); ++move) {
            uint8_t peace_index;
            uint8_t to;

            std::cout << "Take peace(q to cancel all moves): ";
            std::cin >> peace_index;
            while (!backgammon.take_peace(peace_index)) {
                std::cout << "Invalid peace! Take another(q to cancel all moves): ";
                std::cin >> peace_index;
                if (peace_index == 'q') {
                    backgammon.cancel_moves();
                    break;
                }
            }

            std::cout << "Move to(q to cancel all moves): ";
            std::cin >> to;
            while (!backgammon.move_to(peace_index)) {
                std::cout << "Invalid move! Take another(q to cancel all moves): ";
                std::cin >> to;
                if (peace_index == 'q') {
                    backgammon.cancel_moves();
                    break;
                }
            }

        }
    }
    return 0;
}

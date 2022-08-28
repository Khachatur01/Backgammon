#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <chrono>
#include "backgammon.h"

/* private */
int8_t Backgammon::get_move(uint8_t peace, uint8_t step) {
    auto move = int8_t(peace - step);
    if (move < 0) {
        move = -1;
    }
    return move;
}
bool Backgammon::in_range(uint8_t start, uint8_t end, uint8_t number) {
    return number >= start && number < end;
}

void Backgammon::set_available_moves() {
    uint8_t *opponent_peaces;
    switch (this->player) {
        case Player::WHITE:
            opponent_peaces = this->black_peaces;
            break;
        case Player::BLACK:
            opponent_peaces = this->white_peaces;
            break;
        default:
            break;
    }

    if (this->dices.first != this->dices.second) {
        int8_t first_dice_move = Backgammon::get_move(this->selected_peace, this->dices.first);
        int8_t second_dice_move = Backgammon::get_move(this->selected_peace, this->dices.second);
        int8_t combined_dice_move = Backgammon::get_move(this->selected_peace, this->dices.first + this->dices.second);
        if (first_dice_move != -1) {
            if (opponent_peaces[first_dice_move] != 0) {
                this->available_moves.push_back(first_dice_move);
            }
        }
        if (second_dice_move != -1) {
            if (opponent_peaces[second_dice_move] != 0) {
                this->available_moves.push_back(second_dice_move);
            }
        }
        if ((first_dice_move != -1 || second_dice_move != -1) && combined_dice_move != -1) {
            if (opponent_peaces[combined_dice_move] != 0) {
                this->available_moves.push_back(combined_dice_move);
            }
        }
    } else {

    }
}
void Backgammon::clear_board() {
    for (auto & row : this->board) {
        for (auto & col : row) {
            col = this->SPACE;
        }
    }
}
void Backgammon::fill_board() {
    /*
         0   1   2   3   4   5       6   7   8   9   10  11 -> black <--+
    +--< 12  13  14  15  16  17      18  19  20  21  22  23 -> white    |
    |                                                                   |
    |                                                                   |
    |                                                                   |
    |    23  22  21  20  19  18      17  16  15  14  13  12 -> black >--+
    +--> 11  10  9   8   7   6       5   4   3   2   1   0  -> white
     */
    for (uint8_t pip = 0; pip < Backgammon::PIPS_COUNT; ++pip) {
            if (Backgammon::in_range(0, Backgammon::PIPS_COUNT / 2, pip)) {
                for (uint8_t i = 0; i < this->white_peaces[pip]; ++i) {
                    this->board[Backgammon::HEIGHT - 1 - i][Backgammon::PIPS_COUNT / 2 - pip] = Backgammon::WHITE; /* fill to top */
                    if (i == Backgammon::MAX_PEACE) {
                        this->board[Backgammon::HEIGHT - 1 - i][Backgammon::PIPS_COUNT / 2 - pip] = this->NUMBERS[this->white_peaces[pip] - 1];
                        break;
                    }
                }
                for (uint8_t i = 0; i < this->black_peaces[pip]; ++i) {
                    this->board[i][pip] = Backgammon::BLACK; /* fill to bottom */
                    if (i == Backgammon::MAX_PEACE) {
                        this->board[i][pip] = this->NUMBERS[this->white_peaces[pip] - 1];
                        break;
                    }
                }
            } else { /* if (Backgammon::in_range(Backgammon::PIPS_COUNT / 2, Backgammon::PIPS_COUNT, pip)) */
                for (uint8_t i = 0; i < this->white_peaces[pip]; ++i) {
                    this->board[i][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::WHITE; /* fill to bottom */
                    if (i == Backgammon::MAX_PEACE) {
                        this->board[i][pip - Backgammon::PIPS_COUNT / 2] = this->NUMBERS[this->white_peaces[pip] - 1];
                        break;
                    }
                }
                for (uint8_t i = 0; i < this->black_peaces[pip]; ++i) {
                    this->board[Backgammon::HEIGHT - 1 - i][Backgammon::PIPS_COUNT - 1 - pip] = Backgammon::BLACK;  /* fill to top */
                    if (i == Backgammon::MAX_PEACE) {
                        this->board[Backgammon::HEIGHT - 1 - i][Backgammon::PIPS_COUNT - 1 - pip] = this->NUMBERS[this->white_peaces[pip] - 1];
                        break;
                    }
                }
            }
    }
}
void Backgammon::set_dices_on_bord() {
    this->board[Backgammon::HEIGHT / 2 - 1][Backgammon::WIDTH / 4] = this->DICES[this->dices.first - 1];
    this->board[Backgammon::HEIGHT / 2][Backgammon::WIDTH / 4] = this->NUMBERS[this->dices.first - 1];
    this->board[Backgammon::HEIGHT / 2 - 1][Backgammon::WIDTH - Backgammon::WIDTH / 4] = this->DICES[this->dices.second - 1];
    this->board[Backgammon::HEIGHT / 2][Backgammon::WIDTH - Backgammon::WIDTH / 4] = this->NUMBERS[this->dices.second - 1];
}
void Backgammon::render_row(uint8_t start, uint8_t end, uint8_t row) {
    for (uint8_t col = start; col < end; ++col) {
        std::string cell = this->board[row][col];
        if (col == Backgammon::WIDTH / 2 - 1 || col == Backgammon::WIDTH - 1) {
            cell += this->SPACE;
        } else if (row == 0 || row == Backgammon::HEIGHT - 1) {
            cell += this->SPACE + this->SEPARATOR;
        } else {
            cell += this->SPACE + this->SPACE;
        }
        std::cout << cell;
    }
}
void Backgammon::render() {
    system("clear");
    std::cout << "Available moves -> ";
    for (uint8_t move: this->available_moves) {
        std::cout << move << '\t';
    }
    std::cout << std::endl;

    std::cout << "Move -> " << (this->player == Player::WHITE ? this->WHITE : this->player == Player::BLACK ? this->BLACK : "") << std::endl;
    this->clear_board();
    this->fill_board();
    this->set_dices_on_bord();

    /* top index row */
    std::cout << this->SPACE;
    for (uint8_t col = 'a'; col <= 'f'; ++col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << this->SPACE;
    for (uint8_t col = 'g'; col <= 'l'; ++col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << std::endl;

    /* top border */
    for (uint8_t col = 0; col < uint8_t(Backgammon::WIDTH * 3 + 2); ++col) {
        std::cout << this->BORDER;
    }
    std::cout << std::endl;

    for (uint8_t row = 0; row < Backgammon::HEIGHT; ++row) {
        std::cout << this->BORDER; /* left border */
        this->render_row(0, Backgammon::WIDTH / 2, row);
        std::cout << this->BORDER << this->BORDER; /* middle border */
        this->render_row(Backgammon::WIDTH / 2, Backgammon::WIDTH, row);
        std::cout << this->BORDER << std::endl; /* right border */
    }

    /* bottom border */
    for (uint8_t row = 0; row < uint8_t(Backgammon::WIDTH * 3 + 2); ++row) {
        std::cout << this->BORDER;
    }
    std::cout << std::endl;

    /* bottom index row */
    std::cout << this->SPACE;
    for (uint8_t col = 'x'; col >= 's'; --col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << this->SPACE;
    for (uint8_t col = 'r'; col >= 'm'; --col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << std::endl;
}


/* public */
Backgammon::Backgammon() {
    this->white_peaces[23] = 15;
    this->black_peaces[23] = 15;
    this->player = Player::NONE;
    do {
        this->throw_dice();
    } while (this->dices.first == this->dices.second);
    this->player = this->dices.first > this->dices.second ? Player::BLACK : Player::WHITE;
    this->render();
}

void Backgammon::throw_dice() {
    for (uint8_t i = 0; i < Backgammon::DICE_ROTATE_COUNT; ++i) {
        srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        this->dices.first = 1 + rand() % 6;
        this->dices.second = 1 + rand() % 6;
        this->render();

        usleep(Backgammon::THROW_DELAY * 1000);
    }
    this->render();
}

bool Backgammon::take_peace(uint8_t peace_index) {
    uint8_t *peaces = (this->player == Player::WHITE ? this->white_peaces : this->player == Player::BLACK ? this->black_peaces : nullptr);
    if (peace_index - 'a' < Backgammon::PIPS_COUNT && peaces != nullptr && peaces[peace_index - 'a'] != 0) {
        this->selected_peace = peace_index - 'a';
        this->set_available_moves();
        return true;
    }
    return false;
}
bool Backgammon::move_to(uint8_t peace_index) {
    return false;
}
void Backgammon::cancel_moves() {
}

bool Backgammon::is_game_over() const {
    return this->game_over;
}
uint8_t Backgammon::get_available_moves_count() const {
    if (this->dices.first == this->dices.second) {
        return 4;
    }
    return 2;
}

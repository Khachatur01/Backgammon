#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <chrono>
#include "backgammon.h"

/* private */
int8_t Backgammon::get_move(uint8_t peace, uint8_t step, bool to_top_left) {
    int8_t move;
    if (to_top_left) {
        if (Backgammon::in_range(0, Backgammon::pips_count / 2, peace)) { /* if peace in top part */
            move = int8_t(peace - step);
            if (move < 0) {
                move = -1;
            }
        } else if (Backgammon::in_range(Backgammon::pips_count / 2, Backgammon::pips_count, peace)) { /* if peace in bottom part */
            move = int8_t(peace + step);
            if (move >= Backgammon::pips_count) {
                move = int8_t(Backgammon::pips_count / 2 + Backgammon::pips_count - move);
            }
        } else {
            move =  -1;
        }
    } else { /* to_bottom_right */
        if (Backgammon::in_range(0, Backgammon::pips_count / 2, peace)) { /* if peace in top part */
            move = int8_t(peace - step);
            if (move < 0) {
                move = int8_t(Backgammon::pips_count / 2 - move);
            }
        } else if (Backgammon::in_range(Backgammon::pips_count / 2, Backgammon::pips_count, peace)) { /* if peace in bottom part */
            move = int8_t(peace + step);
            if (move >= Backgammon::pips_count) {
                move = -1;
            }
        } else {
            move =  -1;
        }
    }
    return move;
}
bool Backgammon::in_range(uint8_t start, uint8_t end, uint8_t number) {
    return number >= start && number < end;
}

void Backgammon::set_available_moves() {
    switch (this->player) {
        case Player::WHITE:

            break;
        case Player::BLACK:

            break;
        default:
            break;
    }
}
void Backgammon::clear_board() {
    for (auto & row : this->board) {
        for (auto & col : row) {
            col = this->SPACE;
        }
    }
}
void Backgammon::fill_board(uint8_t start, uint8_t end, bool is_top) {
    for (uint8_t j = start; j < end; ++j) {
        uint8_t *current_peaces;
        std::string PEACE;
        if (this->white_peaces[j] != 0) {
            current_peaces = this->white_peaces;
            PEACE = this->WHITE;
        } else if (this->black_peaces[j] != 0) {
            current_peaces = this->black_peaces;
            PEACE = this->BLACK;
        } else {
            continue;
        }

        for (uint8_t i = 0; i < current_peaces[j]; ++i) {
            std::string *cell;
            if (is_top) {
                cell = &this->board[i][j];
            } else {
                cell = &this->board[Backgammon::height - i - 1][j - start];
            }
            if (i > this->MAX_PEACE) {
                *cell = this->NUMBERS[current_peaces[j] - 1];
                break;
            }
            *cell = PEACE;
        }
    }
}
void Backgammon::set_dices_on_bord() {
    this->board[Backgammon::height / 2 - 1][Backgammon::width / 4] = this->DICES[this->dices.first - 1];
    this->board[Backgammon::height / 2][Backgammon::width / 4] = this->NUMBERS[this->dices.first - 1];
    this->board[Backgammon::height / 2 - 1][Backgammon::width - Backgammon::width / 4] = this->DICES[this->dices.second - 1];
    this->board[Backgammon::height / 2][Backgammon::width - Backgammon::width / 4] = this->NUMBERS[this->dices.second - 1];
}
void Backgammon::render_index_row(uint8_t start) {
    std::cout << this->SPACE;
    for (uint8_t col = start; col < start + Backgammon::width / 2; ++col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << this->SPACE;
    for (uint8_t col = start + Backgammon::width / 2; col < start + Backgammon::width; ++col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << std::endl;
}
void Backgammon::render_row(uint8_t start, uint8_t end, uint8_t row) {
    for (uint8_t col = start; col < end; ++col) {
        std::string cell = this->board[row][col];
        if (col == Backgammon::width / 2 - 1 || col == Backgammon::width - 1) {
            cell += this->SPACE;
        } else if (row == 0 || row == Backgammon::height - 1) {
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
    uint8_t peaces_middle = Backgammon::pips_count / 2;
    this->clear_board();
    this->fill_board(0, peaces_middle, true);
    this->fill_board(peaces_middle, Backgammon::pips_count, false);
    this->set_dices_on_bord();

    this->render_index_row('a');
    for (uint8_t col = 0; col < uint8_t(Backgammon::width * 3 + 2); ++col) {
        std::cout << this->BORDER;
    }
    std::cout << std::endl;

    for (uint8_t row = 0; row < Backgammon::height; ++row) {
        std::cout << this->BORDER; /* left border */
        this->render_row(0, Backgammon::width / 2, row);
        std::cout << this->BORDER << this->BORDER; /* middle border */
        this->render_row(Backgammon::width / 2, Backgammon::width, row);
        std::cout << this->BORDER << std::endl; /* right border */
    }

    for (uint8_t row = 0; row < uint8_t(Backgammon::width * 3 + 2); ++row) {
        std::cout << this->BORDER;
    }
    std::cout << std::endl;
    this->render_index_row('m');
}


/* public */
Backgammon::Backgammon() {
    this->white_peaces[11] = 15;
    this->black_peaces[12] = 15;
    this->player = Player::NONE;
    do {
        this->throw_dice();
    } while (this->dices.first == this->dices.second);
    this->player = this->dices.first > this->dices.second ? Player::BLACK : Player::WHITE;
    this->render();
}

void Backgammon::throw_dice() {
    for (uint8_t i = 0; i < Backgammon::dice_rotate_count; ++i) {
        srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        this->dices.first = 1 + rand() % 6;
        this->dices.second = 1 + rand() % 6;
        this->render();

        usleep(Backgammon::throw_delay * 1000);
    }
    this->render();
}

bool Backgammon::take_peace(uint8_t peace_index) {
    uint8_t *peaces = (this->player == Player::WHITE ? this->white_peaces : this->player == Player::BLACK ? this->black_peaces : nullptr);
    if (peace_index - 'a' < Backgammon::pips_count && peaces != nullptr && peaces[peace_index - 'a'] != 0) {
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

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <chrono>
#include <algorithm>
#include "backgammon.h"

/* private */
uint8_t Backgammon::opponent_equivalent_pip(const Player& player, uint8_t pip) {
    if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
         pip += Backgammon::PIPS_COUNT / 2;
    } else if (pip >= Backgammon::PIPS_COUNT / 2 && pip < Backgammon::PIPS_COUNT) {
        pip -= Backgammon::PIPS_COUNT / 2;
    } else {
        pip = Backgammon::PIPS_COUNT;
    }
    return pip;
}

int8_t Backgammon::get_move(uint8_t pip, uint8_t step, const Player& player) {
    auto move = int8_t(pip + step);
    if (
        /* if move out of board */
        (move < 0 || move >= Backgammon::PIPS_COUNT) ||
        /* if opponent don't have peaces in that pip */
        player.opponent->peaces[Backgammon::opponent_equivalent_pip(player, move)] != 0
    ) {
        move = -1;
    }

    return move;
}
void Backgammon::set_available_moves() {
    this->available_moves.clear();
    if (this->selected_peace == Backgammon::PIPS_COUNT || this->player == nullptr) {
        return;
    }
    if (this->dices.first != this->dices.second) {
        int8_t first_dice_move = Backgammon::get_move(this->selected_peace, this->dices.first, *this->player);
        int8_t second_dice_move = Backgammon::get_move(this->selected_peace, this->dices.second, *this->player);
        int8_t combined_dice_move = Backgammon::get_move(this->selected_peace, this->dices.first + this->dices.second, *this->player);

        /* if move can be done and opponent don't have peace on that pip */
        if (first_dice_move != -1) {
            this->available_moves.push_back(first_dice_move);
        }
        /* if move can be done and opponent don't have peace on that pip */
        if (second_dice_move != -1) {
            this->available_moves.push_back(second_dice_move);
        }
        /* if even one move can be done, and opponent don't have peace on that pip */
        if ((first_dice_move != -1 || second_dice_move != -1) && combined_dice_move != -1) {
            this->available_moves.push_back(combined_dice_move);
        }
    } else {
        int8_t move;
        for (uint8_t i = 1; i <= 4; ++i) {
            move = Backgammon::get_move(this->selected_peace, this->dices.first * i, *this->player);
            /* if move can be done and opponent don't have peace on that pip */
            if (move != -1) {
                this->available_moves.push_back(move);
            } else {
                break;
            }
        }
    }
}
void Backgammon::clear_board() {
    for (auto & row : this->board) {
        for (auto & col : row) {
            col = this->SPACE;
        }
    }
}
void Backgammon::set_peaces_on_board(const Player& current_player) {
    /*
             23  22  21  20  19  18      17  16  15  14  13  12 <--+
        +--< 11  10   9   8   7   6       5   4   3   2   1   0    |
        |                                                          |
        |                                                          |
        |                                                          |
        |                                                          |
        |                                                          |
        |                                                          |
        |                                                          |
        |                                                          |
        |                                                          |
        +--> 12  13  14  15  16  17      18  19  20  21  22  23    |
              0   1   2   3   4   5       6   7   8   9  10  11 >--+
     * */

    for (uint8_t pip = 0; pip < Backgammon::PIPS_COUNT; ++pip) {
        if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
            for (uint8_t i = 0; i < current_player.peaces[pip]; ++i) {
                this->board[i][Backgammon::PIPS_COUNT / 2 - pip - 1] = current_player.peace; /* fill to top */
                if (i == Backgammon::MAX_PEACE) {
                    this->board[i][Backgammon::PIPS_COUNT / 2 - pip - 1] = this->NUMBERS[current_player.peaces[pip] - 1];
                    break;
                }
            }
            for (uint8_t i = 0; i < current_player.opponent->peaces[pip]; ++i) {
                this->board[Backgammon::HEIGHT - 1 - i][pip] = current_player.opponent->peace; /* fill to bottom */
                if (i == Backgammon::MAX_PEACE) {
                    this->board[Backgammon::HEIGHT - 1 - i][pip] = this->NUMBERS[current_player.opponent->peaces[pip] - 1];
                    break;
                }
            }
        } else { /* if (pip >= Backgammon::PIPS_COUNT / 2 && pip < Backgammon::PIPS_COUNT) */
            for (uint8_t i = 0; i < current_player.peaces[pip]; ++i) {
                this->board[Backgammon::HEIGHT - 1 - i][pip - Backgammon::PIPS_COUNT / 2] = current_player.peace; /* fill to bottom */
                if (i == Backgammon::MAX_PEACE) {
                    this->board[Backgammon::HEIGHT - 1 - i][pip - Backgammon::PIPS_COUNT / 2] = this->NUMBERS[current_player.peaces[pip] - 1];
                    break;
                }
            }
            for (uint8_t i = 0; i < current_player.opponent->peaces[pip]; ++i) {
                this->board[i][Backgammon::PIPS_COUNT - pip] = current_player.opponent->peace;  /* fill to top */
                if (i == Backgammon::MAX_PEACE) {
                    this->board[i][Backgammon::PIPS_COUNT - pip] = this->NUMBERS[current_player.opponent->peaces[pip] - 1];
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
void Backgammon::set_moves_on_bord() {
    for (uint8_t &pip : this->available_moves) {
        if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
            this->board[this->player->peaces[pip]][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::UP;
        } else {
            this->board[Backgammon::HEIGHT - this->player->peaces[pip] - 1][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::DOWN;
        }
    }
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
    system("clear"); /* todo make cross platform */

    std::cout << "Move -> " << (this->player != nullptr ? this->player->peace : "") << std::endl;
    this->clear_board();
    this->set_peaces_on_board((this->render_for == nullptr) ? this->players[0] : *this->render_for);
    this->set_dices_on_bord();
    this->set_moves_on_bord();

    /* top index row */
    std::cout << this->SPACE;
    for (uint8_t col = 'l'; col >= 'g'; --col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << this->SPACE;
    for (uint8_t col = 'f'; col >= 'a'; --col) {
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
    for (uint8_t col = 'm'; col <= 'r'; ++col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << this->SPACE;
    for (uint8_t col = 's'; col <= 'x'; ++col) {
        std::cout << col << this->SPACE << this->SPACE;
    }
    std::cout << std::endl;
}

/* public */
Backgammon::Backgammon() {
    this->players[0].opponent = &this->players[1];
    this->players[1].opponent = &this->players[0];
}

void Backgammon::start() {
    this->render_for = &this->players[0];
    do {
        this->throw_dice();
    } while (this->dices.first == this->dices.second);
    this->player = this->dices.first > this->dices.second ? &this->players[0] : &this->players[1];
    this->render_for = this->player;
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
}

bool Backgammon::take_peace(uint8_t peace_index) {
    peace_index -= 'a';
    if (peace_index < Backgammon::PIPS_COUNT && this->player->peaces[peace_index] != 0) {
        this->selected_peace = peace_index;
        this->set_available_moves();
        this->render();
        return true;
    }
    return false;
}
bool Backgammon::move_to(uint8_t pip) {
    pip -= 'a';
    if (std::find(this->available_moves.begin(), this->available_moves.end(), pip) != this->available_moves.end()) {
        this->temporarily_moves.emplace_back(this->selected_peace, pip);
        this->player->peaces[this->selected_peace]--;
        this->player->peaces[pip]++;

        this->selected_peace = Backgammon::PIPS_COUNT;
        this->set_available_moves();
        this->render();
        return true;
    }
    return false;
}
bool Backgammon::commit_moves() {
    this->player = this->player->opponent;
    this->render_for = this->player;
    this->selected_peace = Backgammon::PIPS_COUNT; /* no selection */
    this->set_available_moves();
    this->render();
    return true;
}
void Backgammon::cancel_moves() {
    for (std::pair<uint8_t, uint8_t> &move : this->temporarily_moves) {
        this->player->peaces[move.second]--; /* from */
        this->player->peaces[move.first]++; /* to */
    }

    this->selected_peace = Backgammon::PIPS_COUNT; /* no selection */
    this->set_available_moves();
    this->render();
}

bool Backgammon::is_game_over() const {
    return this->game_over;
}
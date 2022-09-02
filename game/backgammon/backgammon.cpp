#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <chrono>
#include <algorithm>
#include "backgammon.h"

/* private */
/* consts */
const std::string Backgammon::NUMBERS[15] = {"⑴", "⑵", "⑶", "⑷", "⑸", "⑹", "⑺", "⑻", "⑼", "⑽", "⑾", "⑿", "⒀", "⒁", "⒂"};
const std::string Backgammon::DICES[6] = {"⚀", "⚁", "⚂", "⚃", "⚄", "⚅"};
const std::string Backgammon::BORDER = "█";
const std::string Backgammon::SPACE = " ";
const std::string Backgammon::SEPARATOR = "|";
const std::string Backgammon::WHITE = "⛂";
const std::string Backgammon::BLACK = "⛀";
const std::string Backgammon::UP = "⬆";
const std::string Backgammon::DOWN = "⬇";
const std::string Backgammon::POSSIBLE_MOVE = "🞙";

uint8_t Backgammon::opponent_pip(const Player& player, uint8_t pip) {
    if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
         pip += Backgammon::PIPS_COUNT / 2;
    } else if (pip >= Backgammon::PIPS_COUNT / 2 && pip < Backgammon::PIPS_COUNT) {
        pip -= Backgammon::PIPS_COUNT / 2;
    } else {
        pip = Backgammon::PIPS_COUNT;
    }
    return pip;
}
int8_t Backgammon::get_move(const Player& player, uint8_t pip, uint8_t step) {
    if (step == 0) {
        return -1;
    }
    auto move = int8_t(pip + step);
    if (
        /* if move out of board */
        (move < 0 || move >= Backgammon::PIPS_COUNT) ||
        /* if opponent don't have peaces in that pip */
        player.opponent->peaces[Backgammon::opponent_pip(player, move)] != 0
    ) {
        move = -1;
    }

    return move;
}
std::vector<uint8_t> Backgammon::get_moves_for_peace(const Player& player, uint8_t peace, std::pair<uint8_t, uint8_t> dices, std::vector<std::pair<uint8_t, uint8_t>> done_moves) {
    std::vector<uint8_t> moves;
    if (peace == 0 && !Backgammon::can_play_from_head(player, done_moves, dices)) { /* when playing from head, and played from head */
        return moves;
    }

    if (peace == Backgammon::PIPS_COUNT) {
        return moves;
    }

    if (dices.first == dices.second) {
        uint8_t available_moves_count = 4;
        for (std::pair<uint8_t, uint8_t> &move: done_moves) {
            uint8_t move_step = move.second - move.first;
            available_moves_count -= move_step / dices.first;
        }
        for (uint8_t i = 0; i < available_moves_count; ++i) {
            int8_t move = Backgammon::get_move(player, peace, dices.first * (i + 1));
            /* if move can be done and opponent don't have peace on that pip */
            if (move != -1) {
                moves.push_back(move);
            } else {
                break;
            }
        }
    } else {
        int8_t first_dice_move = Backgammon::get_move(player, peace, dices.first);
        int8_t second_dice_move = Backgammon::get_move(player, peace, dices.second);
        int8_t combined_dice_move = Backgammon::get_move(player, peace, dices.first + dices.second);

        for (std::pair<uint8_t, uint8_t> &move: done_moves) {
            uint8_t move_step = move.second - move.first;
            /* first dice is played */
            if (move_step == dices.first) {
                first_dice_move = -1;
            }
            /* second dice is played */
            else if (move_step == dices.second) {
                second_dice_move = -1;
            }
            /* both dice is played */
            else if (move_step == dices.first + dices.second) {
                first_dice_move = -1;
                second_dice_move = -1;
                combined_dice_move = -1;
            }
        }

        /* if move can be done and opponent don't have peace on that pip */
        if (first_dice_move != -1) {
            moves.push_back(first_dice_move);
        }
        /* if move can be done and opponent don't have peace on that pip */
        if (second_dice_move != -1) {
            moves.push_back(second_dice_move);
        }
        /* if both moves can be done, it means that combined_dice move can be done */
        if (first_dice_move != -1 && second_dice_move != -1 && combined_dice_move != -1) {
            moves.push_back(combined_dice_move);
        }
    }

    return moves;
}
std::vector<uint8_t> Backgammon::get_all_moves(const Player& player, std::pair<uint8_t, uint8_t> dices, const std::vector<std::pair<uint8_t, uint8_t>>& done_moves) {
    std::vector<uint8_t> moves;
    for (uint8_t pip = 0; pip < Backgammon::PIPS_COUNT; ++pip) {
        /* if player don't have peaces in pip, no need to calculate possible moves for that pip */
        if (player.peaces[pip] == 0) {
            continue;
        }
        std::vector<uint8_t> peace_moves = Backgammon::get_moves_for_peace(player, pip, dices, done_moves);
        if (!peace_moves.empty()) {
            moves.insert(moves.end(), peace_moves.begin(), peace_moves.end());
        }
    }
    return moves;
}
bool Backgammon::have_moves(const Player& player, std::pair<uint8_t, uint8_t> dices, const std::vector<std::pair<uint8_t, uint8_t>>& done_moves) {
    for (uint8_t pip = 0; pip < Backgammon::PIPS_COUNT; ++pip) {
        /* if player have peace(s) in pip and can play that move */
        if (player.peaces[pip] != 0 && !Backgammon::get_moves_for_peace(player, pip, dices, done_moves).empty()) {
            return true;
        }
    }
    return false;
}
bool Backgammon::can_play_from_head(const Player& player, const std::vector<std::pair<uint8_t, uint8_t>>& done_moves, std::pair<uint8_t, uint8_t> dices) {
    if (dices.first == dices.second && player.peaces[0] == player.PEACES_COUNT - 1 && (dices.first == 6 || dices.first == 4 || dices.first == 3)) {
        return true;
    }

    /* if there is move from pip number 0 in done_moves, then played from head */
    return !std::any_of(
            done_moves.begin(),
            done_moves.end(),
            [](const std::pair<uint8_t, uint8_t> &move){return move.first == 0;}
        );
}
void Backgammon::clear_board() {
    for (auto & row : this->board) {
        for (auto & col : row) {
            col = Backgammon::SPACE;
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
                board[i][Backgammon::PIPS_COUNT / 2 - pip - 1] = current_player.peace; /* fill to top */
                if (i == Backgammon::MAX_PEACE) {
                    board[i][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::NUMBERS[current_player.peaces[pip] - 1];
                    break;
                }
            }
            for (uint8_t i = 0; i < current_player.opponent->peaces[pip]; ++i) {
                board[Backgammon::HEIGHT - 1 - i][pip] = current_player.opponent->peace; /* fill to bottom */
                if (i == Backgammon::MAX_PEACE) {
                    board[Backgammon::HEIGHT - 1 - i][pip] = Backgammon::NUMBERS[current_player.opponent->peaces[pip] - 1];
                    break;
                }
            }
        } else { /* if (pip >= Backgammon::PIPS_COUNT / 2 && pip < Backgammon::PIPS_COUNT) */
            for (uint8_t i = 0; i < current_player.peaces[pip]; ++i) {
                board[Backgammon::HEIGHT - 1 - i][pip - Backgammon::PIPS_COUNT / 2] = current_player.peace; /* fill to bottom */
                if (i == Backgammon::MAX_PEACE) {
                    board[Backgammon::HEIGHT - 1 - i][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::NUMBERS[current_player.peaces[pip] - 1];
                    break;
                }
            }
            for (uint8_t i = 0; i < current_player.opponent->peaces[pip]; ++i) {
                board[i][Backgammon::PIPS_COUNT - pip] = current_player.opponent->peace;  /* fill to top */
                if (i == Backgammon::MAX_PEACE) {
                    board[i][Backgammon::PIPS_COUNT - pip] = Backgammon::NUMBERS[current_player.opponent->peaces[pip] - 1];
                    break;
                }
            }
        }
    }

}
void Backgammon::set_dices_on_bord() {
    this->board[Backgammon::HEIGHT / 2 - 1][Backgammon::WIDTH / 4] = Backgammon::DICES[this->dices.first - 1];
    this->board[Backgammon::HEIGHT / 2][Backgammon::WIDTH / 4] = Backgammon::NUMBERS[this->dices.first - 1];
    this->board[Backgammon::HEIGHT / 2 - 1][Backgammon::WIDTH - Backgammon::WIDTH / 4] = Backgammon::DICES[this->dices.second - 1];
    this->board[Backgammon::HEIGHT / 2][Backgammon::WIDTH - Backgammon::WIDTH / 4] = Backgammon::NUMBERS[this->dices.second - 1];
}
void Backgammon::set_moves_on_bord() {
    if (this->available_moves.empty()) {
        for (uint8_t &pip : this->all_moves) {
            if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
                this->board[this->player->peaces[pip]][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::POSSIBLE_MOVE;
            } else {
                this->board[Backgammon::HEIGHT - this->player->peaces[pip] - 1][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::POSSIBLE_MOVE;
            }
        }
    }


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
            cell += Backgammon::SPACE;
        } else if (row == 0 || row == Backgammon::HEIGHT - 1) {
            cell += Backgammon::SPACE + Backgammon::SEPARATOR;
        } else {
            cell += Backgammon::SPACE + Backgammon::SPACE;
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
    std::cout << Backgammon::SPACE;
    for (uint8_t col = 'l'; col >= 'g'; --col) {
        std::cout << col << Backgammon::SPACE << Backgammon::SPACE;
    }
    std::cout << Backgammon::SPACE;
    for (uint8_t col = 'f'; col >= 'a'; --col) {
        std::cout << col << Backgammon::SPACE << Backgammon::SPACE;
    }
    std::cout << std::endl;

    /* top border */
    for (uint8_t col = 0; col < uint8_t(Backgammon::WIDTH * 3 + 2); ++col) {
        std::cout << Backgammon::BORDER;
    }
    std::cout << std::endl;

    for (uint8_t row = 0; row < Backgammon::HEIGHT; ++row) {
        std::cout << Backgammon::BORDER; /* left border */
        this->render_row(0, Backgammon::WIDTH / 2, row);
        std::cout << Backgammon::BORDER << Backgammon::BORDER; /* middle border */
        this->render_row(Backgammon::WIDTH / 2, Backgammon::WIDTH, row);
        std::cout << Backgammon::BORDER << std::endl; /* right border */
    }

    /* bottom border */
    for (uint8_t row = 0; row < uint8_t(Backgammon::WIDTH * 3 + 2); ++row) {
        std::cout << Backgammon::BORDER;
    }
    std::cout << std::endl;

    /* bottom index row */
    std::cout << Backgammon::SPACE;
    for (uint8_t col = 'm'; col <= 'r'; ++col) {
        std::cout << col << Backgammon::SPACE << Backgammon::SPACE;
    }
    std::cout << Backgammon::SPACE;
    for (uint8_t col = 's'; col <= 'x'; ++col) {
        std::cout << col << Backgammon::SPACE << Backgammon::SPACE;
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
    this->all_moves.clear();
    for (uint8_t i = 0; i < Backgammon::DICE_ROTATE_COUNT; ++i) {
        srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        this->dices.first = 1 + rand() % (1 + 5);
        this->dices.second = 1 + rand() % (1 + 5);

        this->render();
        usleep(Backgammon::THROW_DELAY * 1000);
    }
    if (player != nullptr) {
        this->all_moves = this->get_all_moves(
                *this->player,
                this->dices,
                this->done_moves
        );
        this->render();
    }
}

bool Backgammon::take_peace(uint8_t peace_index) {
    peace_index -= 'a';

    /* if played from head, can't take that peace */
    if (peace_index == 0 && !Backgammon::can_play_from_head(*this->player, done_moves, dices))  {
        return false;
    }
    if (peace_index < Backgammon::PIPS_COUNT && this->player->peaces[peace_index] != 0) {
        this->selected_peace = peace_index;
        this->available_moves = this->get_moves_for_peace(
                *this->player,
                this->selected_peace,
                this->dices,
                this->done_moves
        );
        this->render();
        return true;
    }
    return false;
}
bool Backgammon::move_to(uint8_t pip) {
    pip -= 'a';
    if (std::find(this->available_moves.begin(), this->available_moves.end(), pip) != this->available_moves.end()) {
        this->done_moves.emplace_back(this->selected_peace, pip);
        this->player->peaces[this->selected_peace]--;
        this->player->peaces[pip]++;

        this->selected_peace = Backgammon::PIPS_COUNT;
        this->available_moves = this->get_moves_for_peace(
                *this->player,
                this->selected_peace,
                this->dices,
                this->done_moves
        );
        this->all_moves = this->get_all_moves(
                *this->player,
                this->dices,
                this->done_moves
        );
        this->render();
        return true;
    }
    return false;
}
bool Backgammon::commit_moves() {
    if (this->have_moves(*this->player, this->dices, this->done_moves)) {/* have available moves */
        return false;
    }
    this->done_moves.clear();
    this->player = this->player->opponent;
    this->render_for = this->player;
    this->selected_peace = Backgammon::PIPS_COUNT; /* no selection */
    this->available_moves = this->get_moves_for_peace(
            *this->player,
            this->selected_peace,
            this->dices,
            this->done_moves
    );
    this->all_moves = this->get_all_moves(
            *this->player,
            this->dices,
            this->done_moves
    );
    this->render();
    return true;
}
void Backgammon::cancel_moves() {
    for (std::pair<uint8_t, uint8_t> &move : this->done_moves) {
        this->player->peaces[move.second]--; /* from */
        this->player->peaces[move.first]++; /* to */
    }
    this->done_moves.clear();

    this->selected_peace = Backgammon::PIPS_COUNT; /* no selection */
    this->available_moves = this->get_moves_for_peace(
            *this->player,
            this->selected_peace,
            this->dices,
            this->done_moves
    );
    this->all_moves = this->get_all_moves(
            *this->player,
            this->dices,
            this->done_moves
    );
    this->render();
}

bool Backgammon::is_game_over() {
    bool white_win = true;
    bool black_win = true;
    for (uint8_t pip = 0; pip < Backgammon::PIPS_COUNT; ++pip) {
        if (this->players[0].peaces[pip] != 0) {
            white_win = false;
        }
        if (this->players[1].peaces[pip] != 0) {
            black_win = false;
        }
    }

    return white_win || black_win;
}
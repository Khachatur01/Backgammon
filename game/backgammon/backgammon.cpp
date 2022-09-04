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
const std::string Backgammon::UP = "⮝";
const std::string Backgammon::DOWN = "⮟";
const std::string Backgammon::RIGHT = "⮞";
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
std::vector<uint8_t> Backgammon::get_available_pips_for_peace(const Player& player, uint8_t peace, dices_t dices, const std::vector<move_t>& done_moves) {
    std::vector<uint8_t> moves;
    if (peace == 0 && !Backgammon::can_play_from_head(player, dices, done_moves)) { /* when playing from head, and played from head */
        return moves;
    }

    if (peace == Backgammon::PIPS_COUNT) {
        return moves;
    }

    if (dices.first == dices.second) {
        uint8_t available_moves_count = 4;
        for (move_t move: done_moves) {
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

        for (move_t move: done_moves) {
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
        /* if even one move can be done, it means that combined_dice move can be done */
        if ((first_dice_move != -1 || second_dice_move != -1) && combined_dice_move != -1) {
            moves.push_back(combined_dice_move);
        }
    }

    return moves;
}
std::vector<uint8_t> Backgammon::get_all_available_pips(const Player& player, dices_t dices, const std::vector<move_t>& done_moves) {
    std::vector<uint8_t> moves;
    for (uint8_t pip = 0; pip < Backgammon::PIPS_COUNT; ++pip) {
        /* if player don't have peaces in pip, no need to calculate possible moves for that pip */
        if (player.peaces[pip] == 0) {
            continue;
        }
        std::vector<uint8_t> peace_moves = Backgammon::get_available_pips_for_peace(player, pip, dices, done_moves);
        if (!peace_moves.empty()) {
            moves.insert(moves.end(), peace_moves.begin(), peace_moves.end());
        }
    }
    return moves;
}
bool Backgammon::have_moves(const Player& player, dices_t dices, const std::vector<move_t>& done_moves) {
    for (uint8_t pip = 0; pip < Backgammon::PIPS_COUNT; ++pip) {
        /* if player have peace(s) in pip and can play that move */
        if (player.peaces[pip] != 0 && !Backgammon::get_available_pips_for_peace(player, pip, dices, done_moves).empty()) {
            return true;
        }
    }
    return false;
}
bool Backgammon::can_play_from_head(const Player& player, dices_t dices, const std::vector<move_t>& done_moves) {
    if (dices.first == dices.second && player.peaces[0] == player.PEACES_COUNT - 1 && (dices.first == 6 || dices.first == 4 || dices.first == 3)) {
        return true;
    }

    /* if there is move from pip number 0 in done_moves, then played from head */
    return !std::any_of(
            done_moves.begin(),
            done_moves.end(),
            [](const move_t &move){return move.first == 0;}
        );
}
void Backgammon::clear_board(board_t& board) {
    for (auto & row : board) {
        for (auto & col : row) {
            col = Backgammon::SPACE;
        }
    }
}
void Backgammon::set_peaces_on_board(const Player& player, board_t& board) {
    /*
     * matrix indexes
     *        0   1   2   3   4   5       6   7   8   9  10  11

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
            for (uint8_t i = 0; i < player.peaces[pip]; ++i) {
                board[i][Backgammon::PIPS_COUNT / 2 - pip - 1] = player.peace; /* fill to bottom */
                if (i == Backgammon::MAX_PEACE) {
                    board[i][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::NUMBERS[player.peaces[pip] - 1];
                    break;
                }
            }
            for (uint8_t i = 0; i < player.opponent->peaces[pip]; ++i) {
                board[Backgammon::HEIGHT - 1 - i][pip] = player.opponent->peace; /* fill to top */
                if (i == Backgammon::MAX_PEACE) {
                    board[Backgammon::HEIGHT - 1 - i][pip] = Backgammon::NUMBERS[player.opponent->peaces[pip] - 1];
                    break;
                }
            }
        } else { /* if (pip >= Backgammon::PIPS_COUNT / 2 && pip < Backgammon::PIPS_COUNT) */
            for (uint8_t i = 0; i < player.peaces[pip]; ++i) {
                board[Backgammon::HEIGHT - 1 - i][pip - Backgammon::PIPS_COUNT / 2] = player.peace; /* fill to top */
                if (i == Backgammon::MAX_PEACE) {
                    board[Backgammon::HEIGHT - 1 - i][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::NUMBERS[player.peaces[pip] - 1];
                    break;
                }
            }
            for (uint8_t i = 0; i < player.opponent->peaces[pip]; ++i) {
                board[i][Backgammon::PIPS_COUNT - pip - 1] = player.opponent->peace;  /* fill to bottom */
                if (i == Backgammon::MAX_PEACE) {
                    board[i][Backgammon::PIPS_COUNT - pip - 1] = Backgammon::NUMBERS[player.opponent->peaces[pip] - 1];
                    break;
                }
            }
        }
    }

}
void Backgammon::set_dices_on_bord(dices_t dices, board_t& board) {
    board[Backgammon::HEIGHT / 2 - 1][Backgammon::WIDTH / 4] = Backgammon::DICES[dices.first - 1];
    board[Backgammon::HEIGHT / 2][Backgammon::WIDTH / 4] = Backgammon::NUMBERS[dices.first - 1];
    board[Backgammon::HEIGHT / 2 - 1][Backgammon::WIDTH - Backgammon::WIDTH / 4] = Backgammon::DICES[dices.second - 1];
    board[Backgammon::HEIGHT / 2][Backgammon::WIDTH - Backgammon::WIDTH / 4] = Backgammon::NUMBERS[dices.second - 1];
}
void Backgammon::set_moves_on_bord(const Player& player, const std::vector<uint8_t>& available_pips_for_selected_peace, const std::vector<uint8_t>& all_available_pips, board_t& board) {
    if (available_pips_for_selected_peace.empty()) {
        for (uint8_t pip : all_available_pips) {
            if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
                board[player.peaces[pip]][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::POSSIBLE_MOVE;
            } else {
                board[Backgammon::HEIGHT - player.peaces[pip] - 1][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::POSSIBLE_MOVE;
            }
        }
    }

    for (uint8_t pip : available_pips_for_selected_peace) {
        if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
            board[player.peaces[pip]][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::UP;
        } else {
            board[Backgammon::HEIGHT - player.peaces[pip] - 1][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::DOWN;
        }
    }
}
std::string Backgammon::render_row(uint8_t start, uint8_t end, uint8_t row, board_t& board) {
    std::string frame_row;
    for (uint8_t col = start; col < end; ++col) {
        std::string cell = board[row][col];
        if (col == Backgammon::WIDTH / 2 - 1 || col == Backgammon::WIDTH - 1) {
            cell += Backgammon::SPACE;
        } else if (row == 0 || row == Backgammon::HEIGHT - 1) {
            cell += Backgammon::SPACE + Backgammon::SEPARATOR;
        } else {
            cell += Backgammon::SPACE + Backgammon::SPACE;
        }
        frame_row += cell;
    }
    return frame_row;
}
std::string Backgammon::render_frame(const Player& player, const Player& viewer, dices_t dices, const std::vector<uint8_t>& available_pips_for_selected_peace, const std::vector<uint8_t>& all_available_pips) {
    board_t board;
    std::string frame;

    Backgammon::clear_board(board);
    Backgammon::set_peaces_on_board(viewer, board);
    Backgammon::set_dices_on_bord(dices, board);
    Backgammon::set_moves_on_bord(player, available_pips_for_selected_peace, all_available_pips, board);

    /* top index row */
    frame += Backgammon::SPACE;
    for (uint8_t col = 'l'; col >= 'g'; --col) {
        frame += (char)col;
        frame += Backgammon::SPACE + Backgammon::SPACE;
    }
    frame += Backgammon::SPACE;
    for (uint8_t col = 'f'; col >= 'a'; --col) {
        frame += (char)col;
        frame += Backgammon::SPACE + Backgammon::SPACE;
    }
    frame += '\n';

    /* top border */
    for (uint8_t col = 0; col < uint8_t(Backgammon::WIDTH * 3 + 2); ++col) {
        frame += Backgammon::BORDER;
    }
    frame += '\n';

    for (uint8_t row = 0; row < Backgammon::HEIGHT; ++row) {
        frame += Backgammon::BORDER; /* left border */
        frame += render_row(0, Backgammon::WIDTH / 2, row, board);
        frame += Backgammon::BORDER + Backgammon::BORDER; /* middle border */
        frame += render_row(Backgammon::WIDTH / 2, Backgammon::WIDTH, row, board);
        frame += Backgammon::BORDER + '\n'; /* right border */
    }

    /* bottom border */
    for (uint8_t row = 0; row < uint8_t(Backgammon::WIDTH * 3 + 2); ++row) {
        frame += Backgammon::BORDER;
    }
    if (!viewer.can_bear_off) {
        frame += Backgammon::SPACE + Backgammon::RIGHT;
    }
    frame += '\n';

    /* bottom index row */
    frame += Backgammon::SPACE;
    for (uint8_t col = 'm'; col <= 'r'; ++col) {
        frame += (char)col;
        frame += Backgammon::SPACE + Backgammon::SPACE;
    }
    frame += Backgammon::SPACE;
    for (uint8_t col = 's'; col <= 'x'; ++col) {
        frame += (char)col;
        frame += Backgammon::SPACE + Backgammon::SPACE;
    }
    frame += '\n';

    return frame;
}

void Backgammon::render() {
    system("clear");

    std::cout << "Move -> " << (this->player != nullptr ? this->player->peace : "") << std::endl;
    std::cout << Backgammon::render_frame(
            (this->player == nullptr) ? this->players[0] : *this->player,
            (this->viewer == nullptr) ? this->players[0] : *this->viewer,
            this->dices,
            this->available_pips_for_selected_peace,
            this->all_available_pips
    );
}

/* public (interface) */
Backgammon::Backgammon() {
    this->players[0].opponent = &this->players[1];
    this->players[1].opponent = &this->players[0];
}

void Backgammon::start() {
    this->viewer = &this->players[0];
    do {
        this->throw_dice();
    } while (this->dices.first == this->dices.second);
    this->player = this->dices.first > this->dices.second ? &this->players[0] : &this->players[1];
    this->viewer = this->player;
    this->render();
}

std::string Backgammon::get_frame() {
    return "Move -> "+ (this->player != nullptr ? this->player->peace : "") + '\n' +
    Backgammon::render_frame(
        *this->player,
        (this->viewer == nullptr) ? this->players[0] : *this->viewer,
        this->dices,
        this->available_pips_for_selected_peace,
        this->all_available_pips
    );
}

void Backgammon::throw_dice() {
    this->all_available_pips.clear();
    for (uint8_t i = 0; i < Backgammon::DICE_ROTATE_COUNT; ++i) {
        srand(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        this->dices.first = 1 + rand() % (1 + 5);
        this->dices.second = 1 + rand() % (1 + 5);

        this->render();
        usleep(Backgammon::THROW_DELAY * 1000);
    }
    if (player != nullptr) {
        this->all_available_pips = this->get_all_available_pips(
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
    if (peace_index == 0 && !Backgammon::can_play_from_head(*this->player, dices, done_moves))  {
        return false;
    }
    if (peace_index < Backgammon::PIPS_COUNT && this->player->peaces[peace_index] != 0) {
        this->selected_peace = peace_index;
        this->available_pips_for_selected_peace = this->get_available_pips_for_peace(
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
    if (std::find(this->available_pips_for_selected_peace.begin(), this->available_pips_for_selected_peace.end(), pip) != this->available_pips_for_selected_peace.end()) {
        this->done_moves.emplace_back(this->selected_peace, pip);
        this->player->peaces[this->selected_peace]--;
        this->player->peaces[pip]++;

        this->selected_peace = Backgammon::PIPS_COUNT;
        this->available_pips_for_selected_peace = this->get_available_pips_for_peace(
                *this->player,
                this->selected_peace,
                this->dices,
                this->done_moves
        );
        this->all_available_pips = this->get_all_available_pips(
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
    this->player = this->player->opponent; /* swap player */
    this->viewer = this->player;
    this->selected_peace = Backgammon::PIPS_COUNT; /* no selection */

    if (!this->player->can_bear_off) { /* if player can't bear off before commit, check if it is possible after commit */
        this->player->can_bear_off = true;
        for (uint8_t i = 0; i < Backgammon::PIPS_COUNT - 6; ++i) {
            if (this->player->peaces[i] != 0) { /* if there is peace before pip number 18 (not included) */
                this->player->can_bear_off = false;
            }
        }
    }

    this->available_pips_for_selected_peace = this->get_available_pips_for_peace(
            *this->player,
            this->selected_peace,
            this->dices,
            this->done_moves
    );
    this->all_available_pips = this->get_all_available_pips(
            *this->player,
            this->dices,
            this->done_moves
    );
    this->render();
    return true;
}
void Backgammon::cancel_moves() {
    for (move_t &move : this->done_moves) {
        this->player->peaces[move.second]--; /* from */
        this->player->peaces[move.first]++; /* to */
    }
    this->done_moves.clear();

    this->selected_peace = Backgammon::PIPS_COUNT; /* no selection */
    this->available_pips_for_selected_peace = this->get_available_pips_for_peace(
            *this->player,
            this->selected_peace,
            this->dices,
            this->done_moves
    );
    this->all_available_pips = this->get_all_available_pips(
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

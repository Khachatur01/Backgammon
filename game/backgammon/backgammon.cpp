#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <algorithm>
#include <random>
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

void Backgammon::clear_board(board_t& board) {
    for (auto & row : board) {
        for (auto & col : row) {
            col = Backgammon::SPACE;
        }
    }
}
void Backgammon::set_peaces_on_board(const Player& viewer, board_t& board) {
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
            for (uint8_t i = 0; i < viewer.peaces[pip]; ++i) {
                board[i][Backgammon::PIPS_COUNT / 2 - pip - 1] = viewer.PEACE; /* fill to bottom */
                if (i == Backgammon::MAX_PEACE) {
                    board[i][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::NUMBERS[viewer.peaces[pip] - 1];
                    break;
                }
            }
            for (uint8_t i = 0; i < viewer.opponent->peaces[pip]; ++i) {
                board[Backgammon::HEIGHT - 1 - i][pip] = viewer.opponent->PEACE; /* fill to top */
                if (i == Backgammon::MAX_PEACE) {
                    board[Backgammon::HEIGHT - 1 - i][pip] = Backgammon::NUMBERS[viewer.opponent->peaces[pip] - 1];
                    break;
                }
            }
        } else { /* if (pip >= Backgammon::PIPS_COUNT / 2 && pip < Backgammon::PIPS_COUNT) */
            for (uint8_t i = 0; i < viewer.peaces[pip]; ++i) {
                board[Backgammon::HEIGHT - 1 - i][pip - Backgammon::PIPS_COUNT / 2] = viewer.PEACE; /* fill to top */
                if (i == Backgammon::MAX_PEACE) {
                    board[Backgammon::HEIGHT - 1 - i][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::NUMBERS[viewer.peaces[pip] - 1];
                    break;
                }
            }
            for (uint8_t i = 0; i < viewer.opponent->peaces[pip]; ++i) {
                board[i][Backgammon::PIPS_COUNT - pip - 1] = viewer.opponent->PEACE;  /* fill to bottom */
                if (i == Backgammon::MAX_PEACE) {
                    board[i][Backgammon::PIPS_COUNT - pip - 1] = Backgammon::NUMBERS[viewer.opponent->peaces[pip] - 1];
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
void Backgammon::set_moves_on_bord(const Player& player, uint8_t peace, const std::vector<uint8_t>& available_pips_for_selected_peace, const std::vector<uint8_t>& all_available_pips, board_t& board) {
    if (peace == Backgammon::PIPS_COUNT) { /* if no selected PEACE, set all available pips */
        for (uint8_t pip : all_available_pips) {
            if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
                board[player.peaces[pip]][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::POSSIBLE_MOVE;
            } else if (pip >= Backgammon::PIPS_COUNT / 2 && pip < Backgammon::PIPS_COUNT) {
                board[Backgammon::HEIGHT - player.peaces[pip] - 1][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::POSSIBLE_MOVE;
            }
        }
    } else { /* if there is selected PEACE, set only available pips for that PEACE */
        if (peace >= 0 && peace < Backgammon::PIPS_COUNT / 2) { /* show selected PEACE */
            uint8_t row = player.peaces[peace];
            if (row > Backgammon::MAX_PEACE) {
                row = Backgammon::MAX_PEACE + 1;
            }
            board[row][Backgammon::PIPS_COUNT / 2 - peace - 1] = Backgammon::DOWN;
        } else if (peace >= Backgammon::PIPS_COUNT / 2 && peace < Backgammon::PIPS_COUNT) {
            uint8_t row = Backgammon::HEIGHT - player.peaces[peace] - 1;
            if (row < Backgammon::HEIGHT - Backgammon::MAX_PEACE) {
                row = Backgammon::HEIGHT - Backgammon::MAX_PEACE - 1;
            }
            board[row][peace - Backgammon::PIPS_COUNT / 2] = Backgammon::UP;
        }

        /* set arrows to available moves */
        for (uint8_t pip : available_pips_for_selected_peace) {
            if (pip >= 0 && pip < Backgammon::PIPS_COUNT / 2) {
                board[player.peaces[pip]][Backgammon::PIPS_COUNT / 2 - pip - 1] = Backgammon::UP;
            } else if (pip >= Backgammon::PIPS_COUNT / 2 && pip < Backgammon::PIPS_COUNT) {
                board[Backgammon::HEIGHT - player.peaces[pip] - 1][pip - Backgammon::PIPS_COUNT / 2] = Backgammon::DOWN;
            }
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
std::string Backgammon::render_frame(const Player& player, const Player& viewer, uint8_t peace, dices_t dices, const std::vector<uint8_t>& available_pips_for_selected_peace, const std::vector<uint8_t>& all_available_pips) {
    board_t board;
    std::string frame;

    Backgammon::clear_board(board);
    Backgammon::set_peaces_on_board(viewer, board);
    Backgammon::set_dices_on_bord(dices, board);
    if (&player == &viewer) {
        Backgammon::set_moves_on_bord(player, peace, available_pips_for_selected_peace, all_available_pips, board);
    }

    /* top index row */
    frame += Backgammon::repeat(Backgammon::SPACE, 3);
    for (char col = 'l'; col >= 'g'; --col) {
        frame += col;
        frame += Backgammon::repeat(Backgammon::SPACE, 2);
    }
    frame += Backgammon::SPACE;
    for (char col = 'f'; col >= 'a'; --col) {
        frame += col;
        frame += Backgammon::repeat(Backgammon::SPACE, 2);
    }
    frame += '\n';

    /* opponent bore off peaces top border */
    frame += Backgammon::repeat(Backgammon::BORDER, 2);
    /* top border */
    for (uint8_t col = 0; col < uint8_t(Backgammon::WIDTH * 3 + 2); ++col) {
        frame += Backgammon::BORDER;
    }
    /* player bore off peaces top border */
    frame += Backgammon::repeat(Backgammon::BORDER, 2);
    frame += '\n';

    for (uint8_t row = 0; row < Backgammon::HEIGHT; ++row) {
        /* opponent bore off peaces left/right borders and peaces */
        frame += Backgammon::BORDER + (row < viewer.opponent->bore_off_peaces_count ? viewer.opponent->PEACE : Backgammon::SPACE);

        frame += Backgammon::BORDER; /* left border */
        frame += render_row(0, Backgammon::WIDTH / 2, row, board);
        frame += Backgammon::repeat(Backgammon::BORDER, 2); /* middle border */
        frame += render_row(Backgammon::WIDTH / 2, Backgammon::WIDTH, row, board);
        frame += Backgammon::BORDER; /* right border */

        /* player bore off peaces left/right borders and peaces */
        frame += (row >= Backgammon::HEIGHT - viewer.bore_off_peaces_count ? viewer.PEACE : Backgammon::SPACE) + Backgammon::BORDER;
        frame += '\n';
    }

    /* opponent bore off peaces bottom border */
    frame += Backgammon::repeat(Backgammon::BORDER, 2);
    /* bottom border */
    for (uint8_t row = 0; row < uint8_t(Backgammon::WIDTH * 3 + 2); ++row) {
        frame += Backgammon::BORDER;
    }
    /* player bore off peaces bottom border */
    frame += Backgammon::repeat(Backgammon::BORDER, 2);
    /* show arrow right if he can bear off(remove) selected PEACE */
    if (&player == &viewer && viewer.can_bear_off) {
        const std::vector<uint8_t> *pips;
        std::string indicator;
        if (peace == Backgammon::PIPS_COUNT) { /* no selection */
            pips = &all_available_pips;
            indicator = Backgammon::POSSIBLE_MOVE;
        } else {
            pips = &available_pips_for_selected_peace;
            indicator = Backgammon::RIGHT;
        }

        for (uint8_t move: *pips) {
            if (move >= Backgammon::PIPS_COUNT) { /* there is bearing off move */
                frame += Backgammon::SPACE;
                frame += 'z';
                frame += Backgammon::SPACE;
                frame += indicator;
                break;
            }
        }
    }
    frame += '\n';

    /* bottom index row */
    frame += Backgammon::repeat(Backgammon::SPACE, 3);
    for (char col = 'm'; col <= 'r'; ++col) {
        frame += col;
        frame += Backgammon::repeat(Backgammon::SPACE, 2);
    }
    frame += Backgammon::SPACE;
    for (char col = 's'; col <= 'x'; ++col) {
        frame += col;
        frame += Backgammon::repeat(Backgammon::SPACE, 2);
    }
    frame += '\n';

    return frame;
}
std::string Backgammon::repeat(const std::string& text, uint8_t count) {
    std::string result;
    for (uint8_t i = 0; i < count; ++i) {
        result += text;
    }
    return result;
}
bool Backgammon::bear_off() {
    if (this->player->can_bear_off) {
        int8_t bearing_off_move = Move::UNAVAILABLE;
        for (uint8_t &move: this->available_pips_for_selected_peace) {
            if (move >= Backgammon::PIPS_COUNT) { /* bearing off move */
                bearing_off_move = (int8_t)move;
                break;
            }
        }

        if (bearing_off_move != Move::UNAVAILABLE) {
            this->done_moves.emplace_back(this->selected_peace, bearing_off_move);
            this->player->peaces[this->selected_peace]--;
            this->player->bore_off_peaces_count++;
            return true;
        }
    }
    return false;
}

/* public (interface) */
Backgammon::Backgammon() {
    this->white_player.opponent = &this->black_player;
    this->black_player.opponent = &this->white_player;
}

void Backgammon::start(bool render) {
    this->player = &this->white_player;
    do {
        this->throw_dice(render);
    } while (this->dices.first == this->dices.second);
    this->player = this->dices.first > this->dices.second ? &this->white_player : &this->black_player;
    if (render) {
        this->render();
    }
}

std::string Backgammon::get_frame(Player_t viewer_t) {
    return "Move " + Backgammon::RIGHT + " " + (this->player != nullptr ? this->player->PEACE : "") + '\n' +
           Backgammon::render_frame(
        *this->player,
        viewer_t == this->white_player.TYPE ? this->white_player : this->black_player,
        this->selected_peace,
        this->dices,
        this->available_pips_for_selected_peace,
        this->all_available_pips
    );
}
void Backgammon::render(Player_t viewer_t) {
    system("clear");
    std::cout << this->get_frame(viewer_t);
}
void Backgammon::render() {
    system("clear");
    std::cout << this->get_frame(this->player->TYPE);
}

void Backgammon::throw_dice(bool render) {
    this->all_available_pips.clear();
    for (uint8_t i = 0; i < Backgammon::DICE_ROTATE_COUNT; ++i) {

        std::random_device random_device;
        std::default_random_engine engine(random_device());
        std::uniform_int_distribution<uint8_t> uniform_dist(1, 6);

        this->dices.first = uniform_dist(engine);
        this->dices.second = uniform_dist(engine);

        this->render();
        usleep(Backgammon::THROW_DELAY * 1000);
    }
    if (player != nullptr) {
        if (!this->player->can_bear_off) { /* if player can't bear off before commit, check if it is possible after commit */
            this->player->can_bear_off = true;
            for (uint8_t i = 0; i < Backgammon::PIPS_COUNT - 6; ++i) {
                if (this->player->peaces[i] != 0) { /* if there is PEACE before pip number 18 (not included) */
                    this->player->can_bear_off = false;
                }
            }
        }
        this->all_available_pips = this->player->get_all_available_pips(
                this->dices,
                this->done_moves
        );
        if (render) {
            this->render();
        }
    }
}

bool Backgammon::release_peace(bool render) {
    this->selected_peace = Backgammon::PIPS_COUNT;
    this->available_pips_for_selected_peace = this->player->get_available_pips_for_peace(
            this->selected_peace,
            this->dices,
            this->done_moves
    );
    this->all_available_pips = this->player->get_all_available_pips(
            this->dices,
            this->done_moves
    );
    if (!this->player->can_bear_off) { /* if player can't bear off before commit, check if it is possible after commit */
        this->player->can_bear_off = true;
        for (uint8_t i = 0; i < Backgammon::PIPS_COUNT - 6; ++i) {
            if (this->player->peaces[i] != 0) { /* if there is PEACE before pip number 18 (not included) */
                this->player->can_bear_off = false;
            }
        }
    }
    if (render) {
        this->render();
    }
}
bool Backgammon::take_peace(uint8_t peace_index, bool render) {
    peace_index -= 'a';

    /* if played from head, can't take that PEACE */
    if (peace_index == 0 && !this->player->can_play_from_head(dices, done_moves))  {
        return false;
    }
    if (peace_index < Backgammon::PIPS_COUNT && this->player->peaces[peace_index] != 0) {
        this->selected_peace = peace_index;
        this->available_pips_for_selected_peace = this->player->get_available_pips_for_peace(
                this->selected_peace,
                this->dices,
                this->done_moves
        );
        if (render) {
            this->render();
        }
        return true;
    }
    return false;
}
bool Backgammon::move_to(uint8_t pip, bool render) {
    bool done = false;
    if (pip == 'z') {
        bool bored_off = this->bear_off();
        if (bored_off) {
            this->release_peace(render);
        }
        done = bored_off;
    }

    pip -= 'a';
    if (std::find(this->available_pips_for_selected_peace.begin(), this->available_pips_for_selected_peace.end(), pip) != this->available_pips_for_selected_peace.end()) {
        this->done_moves.emplace_back(this->selected_peace, pip);
        this->player->peaces[this->selected_peace]--;
        this->player->peaces[pip]++;
        this->release_peace(render);
        done = true;
    }

    if (done && this->auto_commit && this->all_available_pips.empty()) {
        this->commit_moves(render);
        this->throw_dice(render);
    }
    return done;
}
bool Backgammon::commit_moves(bool render) {
    if (this->player->have_moves(this->dices, this->done_moves)) {/* have available moves */
        return false;
    }
    this->done_moves.clear();

    /* check winner */
    this->winner = this->player; /* temporally set winner as current player */
    for (uint8_t pip = 0; pip < Backgammon::PIPS_COUNT; ++pip) {
        if (this->player->peaces[pip] != 0) {
            this->winner = nullptr; /* if player have peaces, set winner as nullptr */
            break;
        }
    }

    this->player = this->player->opponent; /* swap player */
    this->release_peace(render);

    return true;
}
void Backgammon::cancel_moves(bool render) {
    for (move_t &move : this->done_moves) { /* rollback moves */
        if (move.second >= Backgammon::PIPS_COUNT) {
            this->player->bore_off_peaces_count--; /* from */
        } else {
            this->player->peaces[move.second]--; /* from */
        }
        this->player->peaces[move.first]++; /* to */
    }
    this->done_moves.clear();
    this->release_peace(render);

    if (render) {
        this->render();
    }
}

Player* Backgammon::get_winner() {
    return this->winner;
}

#include <cstdint>
#include <string>
#include <utility>
#include <algorithm>
#include "player.h"
/* private */

/* public */
Player::Player(uint8_t pips_count, std::string peace, Player_t type) : PIPS_COUNT(pips_count), PEACE(std::move(peace)), TYPE(type) {
    this->peaces = new uint8_t[pips_count]{0};
    this->peaces[0] = this->PEACES_COUNT;
}
Player::~Player() {
    delete[] this->peaces;
}

uint8_t Player::opponent_pip(uint8_t pip) const {
    if (pip >= 0 && pip < this->PIPS_COUNT / 2) {
        pip += this->PIPS_COUNT / 2;
    } else if (pip >= this->PIPS_COUNT / 2 && pip < this->PIPS_COUNT) {
        pip -= this->PIPS_COUNT / 2;
    } else {
        pip = this->PIPS_COUNT;
    }
    return pip;
}
int8_t Player::get_move_pip(uint8_t pip, uint8_t step) const {
    if (step == 0) {
        return Move::UNAVAILABLE;
    }
    auto move_pip = int8_t(pip + step);

    if (
        (move_pip < this->PIPS_COUNT && this->opponent->peaces[this->opponent_pip(move_pip)] != 0) || /* opponent have peaces in that pip */
        this->move_blocks_opponent(move_pip) /* after move player will block opponent to go forward */
    ) {
        move_pip = Move::UNAVAILABLE;
    } else if (move_pip >= this->PIPS_COUNT) { /* bearing off(removing) */
        bool have_higher_peaces = false;
        for (uint8_t temp_pip = this->PIPS_COUNT - this->PIPS_COUNT / 4; temp_pip < pip; ++temp_pip) {
            if (this->peaces[temp_pip] != 0) {
                have_higher_peaces = true;
                break;
            }
        }
        /* if player can't bear_off or have higher peaces and step(dice) doesn't equal to selected PEACE's pip */
        if (!this->can_bear_off || (have_higher_peaces && this->PIPS_COUNT - pip != step)) {
            move_pip = Move::UNAVAILABLE;
        }
    }

    return move_pip;
}
std::vector<uint8_t> Player::get_available_pips_for_peace(uint8_t peace, dices_t dices, const std::vector<move_t>& done_moves) const {
    std::vector<uint8_t> moves;

    if (peace == this->PIPS_COUNT || dices.first == 0 || dices.second == 0) {
        return moves;
    }

    if (peace == 0 && !this->can_play_from_head(dices, done_moves)) { /* when playing from head, and played from head */
        return moves;
    }

    if (dices.first == dices.second) {
        uint8_t available_moves_count = 4;
        for (move_t move: done_moves) {
            uint8_t move_step = move.second - move.first;
            available_moves_count -= move_step / dices.first;
        }
        for (uint8_t i = 0; i < available_moves_count; ++i) {
            int8_t move = this->get_move_pip(peace, dices.first * (i + 1));
            /* if move can be done and opponent don't have PEACE on that pip */
            if (move == Move::UNAVAILABLE) {
                break;
            } else {
                moves.push_back(move);
                if (move >= this->PIPS_COUNT) { /* if bored off, stop checking */
                    break;
                }
            }
        }
    } else {
        int8_t first_dice_move = this->get_move_pip(peace, dices.first);
        int8_t second_dice_move = this->get_move_pip(peace, dices.second);
        int8_t combined_dice_move = this->get_move_pip(peace, dices.first + dices.second);

        /* check if dice played */
        for (move_t move: done_moves) {
            uint8_t move_step = move.second - move.first;
            /* first dice is played */
            if (move_step == dices.first) {
                first_dice_move = Move::DONE;
            }
                /* second dice is played */
            else if (move_step == dices.second) {
                second_dice_move = Move::DONE;
            }
                /* both dices are played */
            else if (move_step == dices.first + dices.second) {
                first_dice_move = Move::DONE;
                second_dice_move = Move::DONE;
                combined_dice_move = Move::DONE;
            }
        }

        /* if move can be done but didn't and opponent don't have PEACE on that pip */
        if (first_dice_move != Move::UNAVAILABLE && first_dice_move != Move::DONE) {
            moves.push_back(first_dice_move);
        }
        /* if move can be done but didn't and opponent don't have PEACE on that pip */
        if (second_dice_move != Move::UNAVAILABLE && second_dice_move != Move::DONE) {
            moves.push_back(second_dice_move);
        }
        /* if even one move can be done, but both didn't, and no move is bearing off move, it means that combined_dice move can be done */
        if (
                (first_dice_move != Move::UNAVAILABLE || second_dice_move != Move::UNAVAILABLE) && /* even one move can be done */
                (first_dice_move != Move::DONE && second_dice_move != Move::DONE) && /* both moves aren't done */
                (first_dice_move < this->PIPS_COUNT && second_dice_move < this->PIPS_COUNT) && /* can't bear off(remove) at least by one die */
                combined_dice_move != Move::UNAVAILABLE
                ) {
            moves.push_back(combined_dice_move);
        }
    }

    return moves;
}
std::vector<uint8_t> Player::get_all_available_pips(dices_t dices, const std::vector<move_t>& done_moves) const {
    std::vector<uint8_t> moves;
    for (uint8_t pip = 0; pip < this->PIPS_COUNT; ++pip) {
        /* if player don't have peaces in pip, no need to calculate possible moves for that pip */
        if (this->peaces[pip] == 0) {
            continue;
        }
        std::vector<uint8_t> peace_moves = this->get_available_pips_for_peace(pip, dices, done_moves);
        if (!peace_moves.empty()) {
            moves.insert(moves.end(), peace_moves.begin(), peace_moves.end());
        }
    }
    return moves;
}
bool Player::have_moves(dices_t dices, const std::vector<move_t>& done_moves) const {
    for (uint8_t pip = 0; pip < this->PIPS_COUNT; ++pip) {
        /* if player have PEACE(s) in pip and can play that move */
        if (this->peaces[pip] != 0 && !this->get_available_pips_for_peace(pip, dices, done_moves).empty()) {
            return true;
        }
    }
    return false;
}
bool Player::can_play_from_head(dices_t dices, const std::vector<move_t>& done_moves) const {
    if (dices.first == dices.second && this->peaces[0] == this->PEACES_COUNT - 1 && (dices.first == 6 || dices.first == 4 || dices.first == 3)) {
        return true;
    }

    /* if there is move from pip number 0 in done_moves, then played from head */
    return !std::any_of(
            done_moves.begin(),
            done_moves.end(),
            [](const move_t &move){return move.first == 0;}
    );
}
bool Player::have_peaces_after(uint8_t pip) const {
    for (pip = pip + 1; pip < this->PIPS_COUNT; ++pip) {
        if (this->peaces[pip] != 0) {
            return true;
        }
    }
    return false;
}
bool Player::move_blocks_opponent(uint8_t move_pip) const {
    if (!this->opponent->have_peaces_after(this->opponent_pip(move_pip))) {
        uint8_t continuously_peaces_count = 0; /* number of continuously connected peaces, except moving peace */
        uint8_t temp_pip = move_pip - 1;
        /* check left part of move_pip */
        while (this->peaces[temp_pip] != 0 && temp_pip >= 0 && temp_pip < this->PIPS_COUNT) {
            temp_pip--;
            continuously_peaces_count++;
        }
        temp_pip = move_pip + 1;
        /* check right part of move_pip */
        while (this->peaces[temp_pip] != 0 && temp_pip >= 0 && temp_pip < this->PIPS_COUNT) {
            temp_pip++;
            continuously_peaces_count++;
        }

        if (continuously_peaces_count > 4) {
            return true;
        }
    }
    return false;
}
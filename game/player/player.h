#ifndef BACKGAMMON_PLAYER_H
#define BACKGAMMON_PLAYER_H
#include "../type/types.h"
#include <vector>

class Player {
public:
    Player *opponent = nullptr;
    uint8_t *peaces;
    uint8_t bore_off_peaces_count = 0; /* removed peaces count */
    bool can_bear_off = false;
    const uint8_t PEACES_COUNT = 15;
    const uint8_t PIPS_COUNT;
    const std::string PEACE;

    Player(uint8_t pips_count, std::string peace);

    /**
     * get opponent pip equivalent to player pip
     * */
    [[nodiscard]] uint8_t opponent_pip(uint8_t pip) const;
    /**
     * returns -1 if there is no possible move(when pip gets out of board)
     * */
    [[nodiscard]] int8_t get_move(uint8_t pip, uint8_t step) const;
    /**
     * get available pips for selected PEACE
     * */
    [[nodiscard]] std::vector<uint8_t> get_available_pips_for_peace(uint8_t peace, dices_t dices, const std::vector<move_t>& done_moves) const;
    /**
     * get all available pips for dices
     * */
    [[nodiscard]] std::vector<uint8_t> get_all_available_pips(dices_t dices, const std::vector<move_t>& done_moves) const;
    /**
     * check if player can do move
     * */
    [[nodiscard]] bool have_moves(dices_t dices, const std::vector<move_t>& done_moves) const;
    /**
     * check if player already didn't take PEACE from head.
     * when game just started and dices are equals and can't play all 4 moves, let take from head another one.
     * */
    [[nodiscard]] bool can_play_from_head(dices_t dices, const std::vector<move_t>& done_moves) const;
};

#endif //BACKGAMMON_PLAYER_H

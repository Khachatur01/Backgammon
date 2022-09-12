#ifndef BACKGAMMON_TYPES_H
#define BACKGAMMON_TYPES_H

enum Move {
    UNAVAILABLE = -1,
    DONE = -2,
};

enum Player_t {
    WHITE,
    BLACK
};

typedef std::pair<uint8_t, uint8_t> dices_t;
typedef std::pair<uint8_t, uint8_t> move_t; /* first: from_pip, second: to_pip */

#endif //BACKGAMMON_TYPES_H

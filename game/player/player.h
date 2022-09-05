#ifndef BACKGAMMON_PLAYER_H
#define BACKGAMMON_PLAYER_H

class Player {
public:
    Player *opponent = nullptr;
    uint8_t *peaces;
    uint8_t bore_off_peaces_count = 5; /* removed peaces count */
    bool can_bear_off = false;
    const uint8_t PEACES_COUNT = 15;
    std::string peace;

    Player(uint8_t pips_count, std::string peace);
};

#endif //BACKGAMMON_PLAYER_H

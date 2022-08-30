#ifndef BACKGAMMON_PLAYER_H
#define BACKGAMMON_PLAYER_H

class Player {
public:
    Player *opponent = nullptr;
    uint8_t *peaces;
    std::string peace;

    Player(uint8_t pips_count, std::string peace);
};

#endif //BACKGAMMON_PLAYER_H

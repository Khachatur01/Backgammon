#ifndef BACKGAMMON_BACKGAMMON_H
#define BACKGAMMON_BACKGAMMON_H
#include "../player/player.h"
#include <vector>
#include <array>

class Backgammon {
private:
    static const std::string NUMBERS[15];
    static const std::string DICES[6];
    static const std::string BORDER;
    static const std::string SPACE;
    static const std::string SEPARATOR;
    static const std::string WHITE;
    static const std::string BLACK;
    static const std::string UP;
    static const std::string DOWN;
    static const std::string POSSIBLE_MOVE;

    static const uint8_t HEIGHT = 17;
    static const uint8_t WIDTH = 6 * 2;
    static const uint8_t PIPS_COUNT = 24;
    static const uint8_t MAX_PEACE = 5; /* maximum peaces count in one pip */
    static const uint8_t DICE_ROTATE_COUNT = 3;
    static const uint8_t THROW_DELAY = 200;
    Player players[2] = {
            Player(Backgammon::PIPS_COUNT, Backgammon::WHITE),
            Player(Backgammon::PIPS_COUNT, Backgammon::BLACK)
    };
    Player *player = nullptr;
    Player *render_for = nullptr;
    uint8_t selected_peace = Backgammon::PIPS_COUNT; /* no selection */
    std::vector<uint8_t> available_moves;
    std::vector<uint8_t> all_moves;
    std::vector<std::pair<uint8_t, uint8_t>> done_moves;
    std::pair<uint8_t, uint8_t> dices = std::make_pair(0, 0);
    std::array<std::array<std::string, WIDTH>, HEIGHT> board;

    /**
     * returns -1 if there is no possible move(when pip gets out of board)
     * */
    static int8_t get_move(const Player& player, uint8_t pip, uint8_t step);
    static uint8_t opponent_pip(const Player& player, uint8_t pip);
    static std::vector<uint8_t> get_moves_for_peace(const Player& player, uint8_t peace, std::pair<uint8_t, uint8_t> dices, std::vector<std::pair<uint8_t, uint8_t>> done_moves);
    static std::vector<uint8_t> get_all_moves(const Player& player, std::pair<uint8_t, uint8_t> dices, const std::vector<std::pair<uint8_t, uint8_t>>& done_moves);
    static bool have_moves(const Player& player, std::pair<uint8_t, uint8_t> dices, const std::vector<std::pair<uint8_t, uint8_t>>& done_moves);
    static bool can_play_from_head(const Player& player, const std::vector<std::pair<uint8_t, uint8_t>>& done_moves, std::pair<uint8_t, uint8_t> dices);
    void clear_board();
    void set_peaces_on_board(const Player& current_player);
    void set_dices_on_bord();
    void set_moves_on_bord(); /* set available moves hint to board */
    void render_row(uint8_t start, uint8_t end, uint8_t row);
    void render();

public:
    Backgammon();
    void start();

    void throw_dice();
    bool take_peace(uint8_t peace_index);
    bool move_to(uint8_t pip);
    bool commit_moves();
    void cancel_moves();
    [[nodiscard]] bool is_game_over();
};

#endif //BACKGAMMON_BACKGAMMON_H

#ifndef BACKGAMMON_BACKGAMMON_H
#define BACKGAMMON_BACKGAMMON_H
#include "../player/player.h"
#include <vector>
#include <array>

enum Move {
    UNAVAILABLE = -1,
    DONE = -2,
};

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
    static const std::string RIGHT;
    static const std::string POSSIBLE_MOVE;

    static const uint8_t HEIGHT = 17;
    static const uint8_t WIDTH = 6 * 2;
    static const uint8_t PIPS_COUNT = 24;
    static const uint8_t MAX_PEACE = 5; /* maximum peaces count in one pip */
    static const uint8_t DICE_ROTATE_COUNT = 3;
    static const uint8_t THROW_DELAY = 200;

    typedef std::array<std::array<std::string, Backgammon::WIDTH>, Backgammon::HEIGHT> board_t;
    typedef std::pair<uint8_t, uint8_t> dices_t;
    typedef std::pair<uint8_t, uint8_t> move_t; /* first: from_pip, second: to_pip */

    Player players[2] = {
            Player(Backgammon::PIPS_COUNT, Backgammon::WHITE),
            Player(Backgammon::PIPS_COUNT, Backgammon::BLACK)
    };
    Player *player = nullptr;
    Player *viewer = nullptr;
    uint8_t selected_peace = Backgammon::PIPS_COUNT; /* no selection */
    std::vector<uint8_t> available_pips_for_selected_peace;
    std::vector<uint8_t> all_available_pips;
    std::vector<move_t> done_moves;
    dices_t dices = std::make_pair(0, 0);
    board_t board;

    /**
     * returns -1 if there is no possible move(when pip gets out of board)
     * */
    static int8_t get_move(const Player& player, uint8_t pip, uint8_t step, bool can_bear_off);
    /**
     * get opponent pip equivalent to player pip
     * */
    static uint8_t opponent_pip(const Player& player, uint8_t pip);
    /**
     * get available pips for selected peace
     * */
    static std::vector<uint8_t> get_available_pips_for_peace(const Player& player, uint8_t peace, dices_t dices, const std::vector<move_t>& done_moves, bool can_bear_off);
    /**
     * get all available pips for dices
     * */
    static std::vector<uint8_t> get_all_available_pips(const Player& player, dices_t dices, const std::vector<move_t>& done_moves, bool can_bear_off);
    /**
     * check if player can do move
     * */
    static bool have_moves(const Player& player, dices_t dices, const std::vector<move_t>& done_moves, bool can_bear_off);
    /**
     * check if player already didn't take peace from head.
     * when game just started and dices are equals and can't play all 4 moves, let take from head another one.
     * */
    static bool can_play_from_head(const Player& player, dices_t dices, const std::vector<move_t>& done_moves);
    static void clear_board(board_t& board);
    static void set_dices_on_bord(dices_t dices, board_t& board);
    static void set_moves_on_bord(const Player& player, uint8_t peace, const std::vector<uint8_t>& available_pips_for_selected_peace, const std::vector<uint8_t>& all_available_pips, board_t& board); /* set moves hint to board */
    static void set_peaces_on_board(const Player& player, board_t& board);
    static std::string render_row(uint8_t start, uint8_t end, uint8_t row, board_t& board);
    static std::string render_frame(const Player& player, const Player& viewer, uint8_t peace, dices_t dices, const std::vector<uint8_t>& available_pips_for_selected_peace, const std::vector<uint8_t>& all_available_pips);
    void render();

public:
    Backgammon();
    void start();

    std::string get_frame();
    void throw_dice();
    bool take_peace(uint8_t peace_index);
    bool move_to(uint8_t pip);
    bool commit_moves();
    void cancel_moves();
    [[nodiscard]] bool is_game_over();
};

#endif //BACKGAMMON_BACKGAMMON_H

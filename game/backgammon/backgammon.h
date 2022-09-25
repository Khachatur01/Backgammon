#ifndef BACKGAMMON_H
#define BACKGAMMON_H
#include "../type/types.h"
#include "../player/player.h"
#include <vector>
#include <array>
#include <unordered_map>
#include <functional>

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

    Player white_player = Player(Backgammon::PIPS_COUNT, Backgammon::WHITE, Player_t::WHITE);
    Player black_player = Player(Backgammon::PIPS_COUNT, Backgammon::BLACK, Player_t::BLACK);
    Player* player = nullptr;
    Player* winner = nullptr;
    uint8_t selected_peace = Backgammon::PIPS_COUNT; /* no selection */
    std::vector<uint8_t> available_pips_for_selected_peace;
    std::vector<uint8_t> all_available_pips;
    std::vector<move_t> done_moves;
    dices_t dices = std::make_pair(0, 0);
    board_t board;

    static void clear_board(board_t& board);
    static void set_peaces_on_board(const Player& viewer, board_t& board);
    static void set_dices_on_bord(dices_t dices, board_t& board);
    static void set_moves_on_bord(const Player& player, uint8_t peace, const std::vector<uint8_t>& available_pips_for_selected_peace, const std::vector<uint8_t>& all_available_pips, board_t& board); /* set moves hint to board */
    static std::string render_row(uint8_t start, uint8_t end, uint8_t row, board_t& board);
    static std::string render_frame(const Player& player, const Player& viewer, uint8_t peace, dices_t dices, const std::vector<uint8_t>& available_pips_for_selected_peace, const std::vector<uint8_t>& all_available_pips);
    bool bear_off(); /* remove PEACE */

    static std::string repeat(const std::string& text, uint8_t count);

public:
    bool auto_commit;
    Player_t viewer;

    /**
     * @param auto_commit automatically commit if there is no possible moves
     * @param render_for player who will view board, if render_for is SWITCH, then rotate board for every player
     * */
    explicit Backgammon(bool auto_commit = false, Player_t render_for = Player_t::SWITCH);
    /**
     * @param starter player who will Start game, if starter is SWITCH, starter will defined by dices
     * @param render true / false
     * */
    void start(Player_t starter = Player_t::SWITCH, bool render = true);

    static void clear_screen();
    std::string get_frame(Player_t viewer_t);
    void render(Player_t viewer_t);
    void render();
    dices_t throw_dice(bool render = true, const dices_t* force_dices = nullptr);
    bool take_peace(uint8_t peace_index, bool render = true);
    void release_peace(bool render = true);
    bool move_to(uint8_t pip, bool render = true);
    bool commit_moves(bool render = true);
    void cancel_moves(bool render = true);
    Player* get_player();
    Player* get_winner();
};

#endif //BACKGAMMON_H

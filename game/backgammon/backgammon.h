#ifndef BACKGAMMON_BACKGAMMON_H
#define BACKGAMMON_BACKGAMMON_H
#include "../player/player.h"
#include <vector>

class Backgammon {
private:
    const std::string NUMBERS[15] = {"⑴", "⑵", "⑶", "⑷", "⑸", "⑹", "⑺", "⑻", "⑼", "⑽", "⑾", "⑿", "⒀", "⒁", "⒂"};
    const std::string DICES[6] = {"⚀", "⚁", "⚂", "⚃", "⚄", "⚅"};
    const std::string BORDER = "█";
    const std::string SPACE = " ";
    const std::string SEPARATOR = "|";
    const std::string WHITE = "⛂";
    const std::string BLACK = "⛀";
    const std::string UP = "⬆";
    const std::string DOWN = "⬇";

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
    std::vector<std::pair<uint8_t, uint8_t>> temporarily_moves;
    std::pair<uint8_t, uint8_t> dices = std::make_pair(0, 0);
    std::string board[HEIGHT][WIDTH];
    bool game_over = false;

    /**
     * returns -1 if there is no possible move(when pip gets out of board)
     * */
    static int8_t get_move(uint8_t pip, uint8_t step, const Player& player);
    static uint8_t opponent_equivalent_pip(const Player& player, uint8_t pip);
    void set_available_moves();
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
    [[nodiscard]] bool is_game_over() const;
};

#endif //BACKGAMMON_BACKGAMMON_H

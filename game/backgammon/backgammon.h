#ifndef BACKGAMMON_BACKGAMMON_H
#define BACKGAMMON_BACKGAMMON_H

#include <vector>

enum Player {
    WHITE,
    BLACK,
    NONE
};

class Backgammon {
private:
    static const uint8_t HEIGHT = 17;
    static const uint8_t WIDTH = 6 * 2;
    static const uint8_t PIPS_COUNT = 24;
    static const uint8_t MAX_PEACE = 5; /* maximum peaces count in one pip */
    static const uint8_t DICE_ROTATE_COUNT = 3;
    static const uint8_t THROW_DELAY = 200;
    Player player;
    uint8_t white_peaces[PIPS_COUNT] = {0};
    uint8_t black_peaces[PIPS_COUNT] = {0};
    uint8_t selected_peace;
    std::vector<uint8_t> available_moves;
    std::pair<uint8_t, uint8_t> dices = std::make_pair(0, 0);
    std::string board[HEIGHT][WIDTH];
    bool game_over = false;

    const std::string NUMBERS[15] = {"⑴", "⑵", "⑶", "⑷", "⑸", "⑹", "⑺", "⑻", "⑼", "⑽", "⑾", "⑿", "⒀", "⒁", "⒂"};
    const std::string DICES[6] = {"⚀", "⚁", "⚂", "⚃", "⚄", "⚅"};
    const std::string BORDER = "█";
    const std::string SPACE = " ";
    const std::string SEPARATOR = "|";
    const std::string WHITE = "⛂";
    const std::string BLACK = "⛀";
    const std::string UP = "⭡";
    const std::string DOWN = "⭣";

    /**
     * returns -1 if there is no possible move(when peace gets out of board)
     * */
    static int8_t get_move(uint8_t peace, uint8_t step);
    static bool in_range(uint8_t start, uint8_t end, uint8_t number);
    void set_available_moves();
    void clear_board();
    void fill_board();
    void set_dices_on_bord();
    void render_row(uint8_t start, uint8_t end, uint8_t row);
    void render();

public:
    Backgammon();

    void throw_dice();
    bool take_peace(uint8_t peace_index);
    bool move_to(uint8_t peace_index);
    void cancel_moves();
    [[nodiscard]] bool is_game_over() const;
    [[nodiscard]] uint8_t get_available_moves_count() const;
};

#endif //BACKGAMMON_BACKGAMMON_H

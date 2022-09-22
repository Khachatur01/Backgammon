#ifndef EVENT_HANDLER
#define EVENT_HANDLER
#include <utility>
#include <sstream>

enum Event {
    START,
    THROW_DICES,
    TAKE_PEACE,
    MOVE_TO,
    CANCEL_MOVES,
    RELEASE_PEACE,
    COMMIT_MOVES
};

struct event {
    static constexpr char sep{':'};
    Event id;
    event(): id() {}
    virtual std::string to_string() {
        return std::to_string(id);
    }
};

struct start : public event {
    Player_t starter;
    explicit start(Player_t starter): starter(starter) {
        this->id = Event::START;
    }
};

struct throw_dices : public event {
    uint8_t first;
    uint8_t second;

    throw_dices(uint8_t first, uint8_t second): first(first), second(second) {
        this->id = Event::THROW_DICES;
    }
    std::string to_string() override {
        return event::to_string() + sep + std::to_string(first) + sep + std::to_string(second);
    }
};
struct take_peace : public event {
    uint8_t pip;

    explicit take_peace(uint8_t pip): pip(pip) {
        this->id = Event::TAKE_PEACE;
    }
    std::string to_string() override {
        return event::to_string() + sep + std::to_string(pip);
    }
};
struct move_to : public event {
    uint8_t pip;

    explicit move_to(uint8_t pip): pip(pip) {
        this->id = Event::MOVE_TO;
    }
    std::string to_string() override {
        return event::to_string() + sep + std::to_string(pip);
    }
};
struct cancel_moves : public event {
    cancel_moves() {
        this->id = Event::CANCEL_MOVES;
    }
};
struct release_peace : public event {
    release_peace() {
        this->id = Event::RELEASE_PEACE;
    }
};
struct commit_moves : public event {
    commit_moves() {
        this->id = Event::COMMIT_MOVES;
    }
};

class EventHandler {
private:
    Backgammon backgammon;
    static std::vector<std::string> split(const std::string& string, char sep) {
        std::stringstream string_stream(string);
        std::string segment;
        std::vector<std::string> list;

        while(std::getline(string_stream, segment, sep)) {
            list.push_back(segment);
        }
        return list;
    }
public:
    explicit EventHandler(bool auto_commit, Player_t render_for): backgammon(Backgammon(auto_commit, render_for)) {}

    event* parse(const std::string& message) {
        event* event = nullptr;
        std::vector<std::string> event_list = EventHandler::split(message, event::sep);
        uint8_t id = std::stoi(event_list[0]);
        switch (id) {
            case START: {
                event = new start(static_cast<Player_t>(std::stoi(event_list[1])));
                break;
            }
            case THROW_DICES: {
                event = new throw_dices(std::stoi(event_list[1]), std::stoi(event_list[2]));
                break;
            }
            case TAKE_PEACE: {
                event = new take_peace(std::stoi(event_list[0]));
                break;
            }
            case MOVE_TO: {
                event = new move_to(std::stoi(event_list[0]));
                break;
            }
            case CANCEL_MOVES: {
                event = new cancel_moves();
                break;
            }
            case RELEASE_PEACE: {
                event = new release_peace();
                break;
            }
            case COMMIT_MOVES: {
                event = new commit_moves();
                break;
            }
            default:
                /* throw exception */
                break;
        }

        return event;
    }

    void handle(const std::string& event) {
        this->handle(parse(event));
    }
    void handle(event* event) {
        switch (event->id) {
            case START: {
                auto* start_event = dynamic_cast<start*>(event);
                this->backgammon.start(start_event->starter);
                this->backgammon.throw_dice();
                break;
            }
            case THROW_DICES: {
                auto* throw_dices_event = dynamic_cast<throw_dices*>(event);
                dices_t force_dices = std::make_pair(throw_dices_event->first, throw_dices_event->first);
                this->backgammon.throw_dice(true, &force_dices);
                break;
            }
            case TAKE_PEACE: {
                auto* take_peace_event = dynamic_cast<take_peace*>(event);
                this->backgammon.take_peace(take_peace_event->pip, true);
                break;
            }
            case MOVE_TO: {
                auto* move_to_event = dynamic_cast<move_to*>(event);
                this->backgammon.take_peace(move_to_event->pip, true);
                break;
            }
            case CANCEL_MOVES: {
                this->backgammon.cancel_moves();
                break;
            }
            case RELEASE_PEACE: {
                this->backgammon.release_peace();
                break;
            }
            case COMMIT_MOVES: {
                this->backgammon.commit_moves();
                break;
            }
        }
    }

    Backgammon* get_backgammon() {
        return &this->backgammon;
    }
};

#endif //EVENT_HANDLER
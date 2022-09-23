#ifndef EVENT_HANDLER
#define EVENT_HANDLER
#include <utility>
#include <sstream>
#include "../../../type/types.h"

enum Event {
    CREATE_ROOM,
    CONNECT_ROOM,
    SET_MYSELF,
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
    Player_t owner;
    std::string room;
    std::string password;

    event(): id(), owner(Player_t::SWITCH) {};
    event(Player_t owner, std::string room, std::string password): id(), owner(owner), room(std::move(room)), password(std::move(password)) {}
    virtual std::string to_string() {
        return std::to_string(id) + sep + std::to_string(this->owner) + this->room + sep + this->password;
    }
    virtual void parse(std::vector<std::string> fields) {
        this->owner = static_cast<Player_t>(std::stoi(fields[1]));
        this->room = fields[2];
        this->password = fields[3];
    }

    static std::vector<std::string> split(const std::string& string, char separator) {
        std::stringstream string_stream(string);
        std::string segment;
        std::vector<std::string> list;
        while (std::getline(string_stream, segment, separator)) {
            list.push_back(segment);
        }
        return list;
    }

    static event* parse(const std::string& data);
};
struct create_room : public event {
    create_room() = default;
    explicit create_room(Player_t owner, const std::string& room, const std::string& password) : event(owner, room, password) {
        this->id = Event::CREATE_ROOM;
    }
    std::string to_string() override {
        return event::to_string();
    }
};
struct connect_room : public event {
    connect_room() = default;
    explicit connect_room(Player_t owner, const std::string& room, const std::string& password) : event(owner, room, password) {
        this->id = Event::CONNECT_ROOM;
    }
    std::string to_string() override {
        return event::to_string();
    }
};
struct set_myself : public event {
    Player_t me = Player_t::SWITCH;

    set_myself() = default;
    explicit set_myself(Player_t owner, const std::string& room, const std::string& password, Player_t me): event(owner, room, password), me(me) {
        this->id = Event::SET_MYSELF;
    }
    std::string to_string() override {
        return event::to_string() + sep + std::to_string(this->me);
    }
    void parse(std::vector<std::string> fields) override {
        event::parse(fields);
        this->me = static_cast<Player_t>(std::stoi(fields[4]));
    }
};
struct start : public event {
    Player_t starter = Player_t::SWITCH;

    start() = default;
    explicit start(Player_t owner, const std::string& room, const std::string& password, Player_t starter): event(owner, room, password), starter(starter) {
        this->id = Event::START;
    }
    std::string to_string() override {
        return event::to_string() + sep + std::to_string(this->starter);
    }
    void parse(std::vector<std::string> fields) override {
        this->starter = static_cast<Player_t>(std::stoi(fields[4]));
    }
};
struct throw_dices : public event {
    uint8_t first = 0;
    uint8_t second = 0;

    throw_dices() = default;
    throw_dices(Player_t owner, const std::string& room, const std::string& password, uint8_t first, uint8_t second): event(owner, room, password), first(first), second(second) {
        this->id = Event::THROW_DICES;
    }
    std::string to_string() override {
        return event::to_string() + sep + std::to_string(first) + sep + std::to_string(second);
    }
    void parse(std::vector<std::string> fields) override {
        this->first = std::stoi(fields[4]);
        this->second = std::stoi(fields[5]);
    }
};
struct take_peace : public event {
    uint8_t pip = 0;

    take_peace() = default;
    explicit take_peace(Player_t owner, const std::string& room, const std::string& password, uint8_t pip): event(owner, room, password), pip(pip) {
        this->id = Event::TAKE_PEACE;
    }
    std::string to_string() override {
        return event::to_string() + sep + std::to_string(pip);
    }
    void parse(std::vector<std::string> fields) override {
        this->pip = std::stoi(fields[4]);
    }
};
struct move_to : public event {
    uint8_t pip = 0;

    move_to() = default;
    explicit move_to(Player_t owner, const std::string& room, const std::string& password, uint8_t pip): event(owner, room, password), pip(pip) {
        this->id = Event::MOVE_TO;
    }
    std::string to_string() override {
        return event::to_string() + sep + std::to_string(pip);
    }
    void parse(std::vector<std::string> fields) override {
        this->pip = std::stoi(fields[4]);
    }
};
struct cancel_moves : public event {
    cancel_moves() = default;
    cancel_moves(Player_t owner, const std::string& room, const std::string& password) : event(owner, room, password) {
        this->id = Event::CANCEL_MOVES;
    }
};
struct release_peace : public event {
    release_peace() = default;
    release_peace(Player_t owner, const std::string& room, const std::string& password) : event(owner, room, password) {
        this->id = Event::RELEASE_PEACE;
    }
};
struct commit_moves : public event {
    commit_moves() = default;
    commit_moves(Player_t owner, const std::string& room, const std::string& password) : event(owner, room, password) {
        this->id = Event::COMMIT_MOVES;
    }
};


event* event::parse(const std::string& data) {
        event* event = nullptr;

        std::vector<std::string> event_list = event::split(data, event::sep);
        auto id = static_cast<Event>(std::stoi(event_list[0]));
        switch (id) {
            case Event::CREATE_ROOM: {
                event = new create_room();
                break;
            }
            case Event::CONNECT_ROOM: {
                event = new connect_room();
                break;
            }
            case Event::SET_MYSELF: {
                event = new set_myself();
                break;
            }
            case Event::START: {
                event = new start();
                break;
            }
            case Event::THROW_DICES: {
                event = new throw_dices();
                break;
            }
            case Event::TAKE_PEACE: {
                event = new take_peace();
                break;
            }
            case Event::MOVE_TO: {
                event = new move_to();
                break;
            }
            case Event::CANCEL_MOVES: {
                event = new cancel_moves();
                break;
            }
            case Event::RELEASE_PEACE: {
                event = new release_peace();
                break;
            }
            case Event::COMMIT_MOVES: {
                event = new commit_moves();
                break;
            }
        }

        if (event) {
            event->parse(event_list);
        }
        return event;
}
#endif //EVENT_HANDLER
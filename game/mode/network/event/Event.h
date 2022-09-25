#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <string>
#include <sstream>
#include "../../../type/types.h"

enum Event_t {
    CREATE_ROOM,
    CONNECT_ROOM,
    CREATE_ROOM_AND_CONNECT,
    SET_MYSELF,
    START,
    THROW_DICES,
    TAKE_PEACE,
    MOVE_TO,
    CANCEL_MOVES,
    RELEASE_PEACE,
    COMMIT_MOVES
};
namespace event {
    struct Event {
        static constexpr char sep{':'};
        Event_t id;
        std::string room;
        std::string password;

        explicit Event(Event_t id);
        Event(Event_t id, std::string room, std::string password);

        virtual std::string to_string();
        virtual void parse(std::vector<std::string> fields);
    };
    struct CreateRoom : public Event {
        CreateRoom();
        CreateRoom(const std::string& room, const std::string& password);
    };
    struct ConnectRoom : public Event {
        ConnectRoom();
        ConnectRoom(const std::string& room, const std::string& password);
    };
    struct CreateRoomAndConnect : public Event {
        CreateRoomAndConnect();
        CreateRoomAndConnect(const std::string& room, const std::string& password);
    };
    struct SetMyself : public Event {
        Player_t me = Player_t::SWITCH;

        SetMyself();
        SetMyself(const std::string& room, const std::string& password, Player_t me);
        std::string to_string() override;
        void parse(std::vector<std::string> fields) override;
    };
    struct Start : public Event {
        Player_t starter = Player_t::SWITCH;

        Start();
        Start(const std::string& room, const std::string& password, Player_t starter);
        std::string to_string() override;
        void parse(std::vector<std::string> fields) override;
    };
    struct ThrowDices : public Event {
        uint8_t first = 0;
        uint8_t second = 0;

        ThrowDices();
        ThrowDices(const std::string& room, const std::string& password, uint8_t first, uint8_t second);
        std::string to_string() override;
        void parse(std::vector<std::string> fields) override;
    };
    struct TakePeace : public Event {
        uint8_t pip = 0;

        TakePeace();
        TakePeace(const std::string& room, const std::string& password, uint8_t pip);
        std::string to_string() override;
        void parse(std::vector<std::string> fields) override;
    };
    struct MoveTo : public Event {
        uint8_t pip = 0;

        MoveTo();
        MoveTo(const std::string& room, const std::string& password, uint8_t pip);
        std::string to_string() override;
        void parse(std::vector<std::string> fields) override;
    };
    struct CancelMoves : public Event {
        CancelMoves();
        CancelMoves(const std::string& room, const std::string& password);
    };
    struct ReleasePeace : public Event {
        ReleasePeace();
        ReleasePeace(const std::string& room, const std::string& password);
    };
    struct CommitMoves : public Event {
        CommitMoves();
        CommitMoves(const std::string& room, const std::string& password);
    };

    Event* parse_message(const std::string& data);

    std::vector<std::string> split(const std::string& string, char separator);
}
#endif //EVENT_H

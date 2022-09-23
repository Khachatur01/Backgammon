#ifndef BACKGAMMON_SERVER_H
#define BACKGAMMON_SERVER_H

#include <cstdint>
#include <sstream>
#include <unordered_map>
#include "../../../type/types.h"
#include "../../../../socket/server/ServerSocket.h"
#include "../../../backgammon/backgammon.h"
#include "event.cpp"

struct RoomPlayer {
    Client client_socket;
    Player_t player;
};
class Room {
private:
    Backgammon backgammon;
    std::string password;
public:
    RoomPlayer* white_player = nullptr;
    RoomPlayer* black_player = nullptr;

    explicit Room(const std::string& password);
    friend class BackgammonServer;
};

class BackgammonServer {
private:
    std::unordered_map<std::string, Room> rooms;
    ServerSocket serverSocket;

public:
    BackgammonServer();
    std::thread run(uint16_t port, uint64_t max_rooms = 0);

    void on_event(const std::string& message);
    void on_event(event* event);
};
#endif //BACKGAMMON_SERVER_H

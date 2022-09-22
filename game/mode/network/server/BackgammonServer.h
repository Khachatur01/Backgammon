#ifndef BACKGAMMON_SERVER_H
#define BACKGAMMON_SERVER_H

#include <cstdint>
#include <unordered_map>
#include "../../../type/types.h"
#include "../../../../socket/server/ServerSocket.h"
#include "../../../backgammon/backgammon.h"
#include "../client/BackgammonClient.h"
#include "../event_handler.cpp"

class BackgammonServer {
private:
    EventHandler eventHandler;
    ServerSocket serverSocket;
    std::unordered_map<Player_t, Client> players_map;

public:
    BackgammonServer();
    void run(uint16_t port);
};
#endif //BACKGAMMON_SERVER_H

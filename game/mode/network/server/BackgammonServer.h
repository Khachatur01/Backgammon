#ifndef BACKGAMMON_SERVER_H
#define BACKGAMMON_SERVER_H

#include <unordered_map>
#include "../../../../socket/server/ServerSocket.h"
#include "../event/Event.h"

class Room {
private:
    std::string password;
public:
    Client* white_player = nullptr;
    Client* black_player = nullptr;

    explicit Room(const std::string& password);
    friend class BackgammonServer;
};

class BackgammonServer {
private:
    std::unordered_map<std::string, Room*> rooms;
    ServerSocket* serverSocket;

    void on_event(Client sender, const std::string& message);
    void on_event(Client sender, event::Event* event);
public:
    BackgammonServer();
    std::thread run(uint16_t port, uint64_t max_rooms = 0);
};
#endif //BACKGAMMON_SERVER_H

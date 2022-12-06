#ifndef BACKGAMMON_SERVER_H
#define BACKGAMMON_SERVER_H

#include <unordered_map>
#include "../../../../socket/server/Server.h"
#include "../event/Event.h"

class Room {
private:
    std::string password;
public:
    Client_t* white_player = nullptr;
    Client_t* black_player = nullptr;

    explicit Room(const std::string& password);
    friend class BackgammonServer;
};

class BackgammonServer {
private:
    std::unordered_map<std::string, Room*> rooms;
    Socket::Server* serverSocket;

    void on_message(Client_t sender, const std::string& message);
    void on_message(Client_t sender, event::Event* event);
public:
    BackgammonServer();
    ~BackgammonServer();
    std::thread run(uint16_t port, uint64_t max_rooms = 0);
};
#endif //BACKGAMMON_SERVER_H

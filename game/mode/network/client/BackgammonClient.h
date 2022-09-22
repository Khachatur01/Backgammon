#ifndef BACKGAMMON_CLIENT_H
#define BACKGAMMON_CLIENT_H

#include "../../../backgammon/backgammon.h"
#include "../../../../socket/client/ClientSocket.h"
#include "../event_handler.cpp"

class BackgammonClient {
private:
    EventHandler eventHandler;
    ClientSocket clientSocket;
public:
    BackgammonClient();
    void connect(const std::basic_string<char> &hostname, uint16_t port);
};

#endif //BACKGAMMON_CLIENT_H

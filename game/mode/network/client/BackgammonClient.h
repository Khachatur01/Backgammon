#ifndef BACKGAMMON_CLIENT_H
#define BACKGAMMON_CLIENT_H

#include "../../../backgammon/backgammon.h"
#include "../../../../socket/client/ClientSocket.h"

class BackgammonClient {
private:
    Backgammon backgammon;
    ClientSocket clientSocket;
public:
    BackgammonClient();
    void connect(const std::basic_string<char> &hostname, uint16_t port);
};

#endif //BACKGAMMON_CLIENT_H

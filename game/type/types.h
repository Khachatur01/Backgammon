#ifndef BACKGAMMON_TYPES_H
#define BACKGAMMON_TYPES_H

#include <functional>
#include <netinet/in.h>

struct Client_t {
    int socket_fd;
    sockaddr_in address;
    bool operator==(const Client_t& client) const {
        return client.socket_fd == this->socket_fd;
    }
    bool operator!=(const Client_t& client) const {
        return client.socket_fd != this->socket_fd;
    }
};

enum Move {
    UNAVAILABLE = -1,
    DONE = -2,
};

enum Player_t {
    SWITCH = 0,
    WHITE = 1,
    BLACK = 2
};

typedef std::pair<uint8_t, uint8_t> dices_t;
typedef std::pair<uint8_t, uint8_t> move_t; /* first: from_pip, second: to_pip */

/* sockets */
typedef std::function<void(const std::string& data)> message_callback;
typedef std::function<void()> disconnect_callback;

typedef std::function<void(const std::string& data, Client_t from)> message_from_callback;
typedef std::function<void(Client_t client)> disconnect_from_callback;
typedef std::function<void(Client_t client)> connect_to_callback;

#endif //BACKGAMMON_TYPES_H

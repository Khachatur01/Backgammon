#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <cstdint>
#include <string>
#include <functional>
#include "../../game/type/types.h"

typedef std::function<void(const std::string& data)> message_callback;
typedef std::function<void()> disconnect_callback;

class ClientSocket {
private:
    int socket_fd;

    sockaddr_in server_address{};
    hostent *server;

    THREAD ON_MESSAGE_THREAD_RUN = THREAD::DETACH;
public:
    message_callback on_message_callback;
    disconnect_callback on_disconnect_callback;

    ClientSocket(const std::string& hostname, uint16_t port);
    ~ClientSocket();
    bool connect();
    [[nodiscard]] bool send(const std::string& data) const;
    void wait_message(size_t buffer_size, THREAD on_message_thread_run);
    void close() const;
};

#endif //CLIENT_SOCKET_H

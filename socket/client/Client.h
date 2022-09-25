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
#include <thread>
#include "../../game/type/types.h"

namespace Socket {
    class Client {
    private:
        int socket_fd;

        sockaddr_in server_address{};
        hostent *server;

    public:
        message_callback on_message;
        disconnect_callback on_disconnect;

        ~Client();

        void create();
        void init_host(const std::string& hostname, uint16_t port);

        bool connect();
        void send(const std::string& data) const;
        std::thread wait_message(size_t buffer_size);
        void close() const;
    };
}

#endif //CLIENT_SOCKET_H

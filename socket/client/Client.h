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

        /**
         * destructor closes socket
         * */
        ~Client();

        /**
         * create and init socket\n
         * throws socket_init_exception
         * */
        void create();

        /**
         * @param hostname host ip
         * @param port host port
         *
         * throws socket_init_exception
         * */
        void init_host(const std::string& hostname, uint16_t port);

        /**
         * try to connect to server, if can't return false
         * */
        bool connect();

        /**
         * @param data message to send to server
         * */
        void send(const std::string& data) const;

        /**
         * @param buffer_size incoming messages size in bytes
         *
         * calls on_message callback when server sends message\n
         * calls on_disconnect callback when server disconnects
         * */
        std::thread wait_message(size_t buffer_size);

        /**
         * close socket connection and shut down socket
         * */
        void close() const;
    };
}

#endif //CLIENT_SOCKET_H

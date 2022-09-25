#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <cstdint>
#include <netinet/in.h>
#include <functional>
#include <thread>
#include <mutex>
#include <list>
#include "../../game/type/types.h"

namespace Socket {
    class Server {
    private:
        int socket_fd;
        sockaddr_in address{};
        std::list<Client_t> clients;
        std::mutex mutex;

    public:
        uint8_t max_connections;
        connect_to_callback on_connect;
        disconnect_from_callback on_disconnect;
        message_from_callback on_message_from;

        /**
         * @param port run socket on this port
         * @param backlog_queue_size max clients waiting for connection at the same time
         * @param max_connections max clients. 0 for infinity connections
         * */
        explicit Server(uint8_t max_connections = 0);
        ~Server();

        void create();
        void bind(uint16_t port);
        void listen(uint8_t backlog_queue_size) const;

        std::thread wait_connection();
        std::thread wait_message_from(Client_t client, size_t buffer_size);
        void broadcast(const std::string& data) const;
        void send_to(Client_t client, const std::string &data) const;
        void close() const;
    };
}

#endif //SERVER_SOCKET_H

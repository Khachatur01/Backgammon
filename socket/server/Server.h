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
        explicit Server();

        /**
         * destructor closes socket
         * */
        ~Server();

        /**
         * create and init socket\n
         * throws socket_creation_exception
         * */
        void create();
        /**
         * @param port bind socket on this port
         *
         * throws socket_bind_exception
         * */
        void bind(uint16_t port);

        /**
         * @param backlog_queue_size how many clients will wait to connect
         * */
        void listen(uint8_t backlog_queue_size) const;

        /**
         * wait connection in parallel thread
         *
         * calls on_connect callback when client connects
         * */
        std::thread wait_connection();

        /**
         * @param client wait message from specific client
         * @param buffer_size incoming messages size in bytes
         *
         * calls on_message_from callback when client sends message\n
         * calls on_disconnect callback when client disconnects
         * */
        std::thread wait_message_from(Client_t client, size_t buffer_size);

        /**
         * @param data send message to all connected clients
         * */
        [[maybe_unused]] void broadcast(const std::string& data) const;

        /**
         * @param data message to send
         * @param client specific client to send message
         * */
        void send_to(Client_t client, const std::string &data) const;

        /**
         * close socket connection with all clients and shut down socket
         * */
        void close() const;

        /**
         * close socket connection with specific client
         * */
        void close(Client_t client);
    };
}

#endif //SERVER_SOCKET_H

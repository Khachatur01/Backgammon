#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <cstdint>
#include <netinet/in.h>
#include <functional>
#include <thread>
#include <mutex>
#include <list>
#include "../../game/type/types.h"

class ServerSocket {
private:
    int socket_fd;
    sockaddr_in address{};
    std::list<Client> clients;
    std::mutex mutex;
    uint8_t max_connections;

public:
    connect_to_callback on_connect;
    disconnect_from_callback on_disconnect;
    message_from_callback on_message_from;

    /**
     * @param port run socket on this port
     * @param backlog_queue_size max clients waiting for connection at the same time
     * @param max_connections max clients. 0 for infinity connections
     * */
    explicit ServerSocket(uint8_t max_connections = 0);
    ~ServerSocket();

    void create();
    void bind(uint16_t port);
    void listen(uint8_t backlog_queue_size) const;

    std::thread wait_connection();
    std::thread wait_message_from(Client client, size_t buffer_size);
    void broadcast(const std::string& data) const;
    void send_to(Client client, const std::string &data) const;
    void close() const;
};

#endif //SERVER_SOCKET_H

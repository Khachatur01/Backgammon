#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <cstdint>
#include <netinet/in.h>
#include <mutex>
#include <functional>
#include <thread>
#include <list>
#include "../../game/type/types.h"

struct Client {
    int socket_fd;
    sockaddr_in address;
    bool operator==(const Client& client) const;
    bool operator!=(const Client& client) const;
};

typedef std::function<void(const std::string& data, Client from)> message_callback;
typedef std::function<void(Client client)> connect_callback;
typedef std::function<void(Client client)> disconnect_callback;

class ServerSocket {
private:
    int socket_fd;
    sockaddr_in address{};
    std::list<Client> clients;
    std::mutex mutex;
    uint8_t max_connections;

    THREAD ON_CONNECT_THREAD_RUN = THREAD::DETACH;
    THREAD ON_MESSAGE_THREAD_RUN = THREAD::DETACH;
public:
    connect_callback on_connect_callback;
    disconnect_callback on_disconnect_callback;
    message_callback on_message_from_callback;

    /**
     * @param port run socket on this port
     * @param backlog_queue_size max clients waiting for connection at the same time
     * @param max_connections max clients. 0 for infinity connections
     * */
    ServerSocket(uint16_t port, uint8_t backlog_queue_size, uint8_t max_connections);
    ~ServerSocket();

    void wait_connection(THREAD on_connect_thread_run);
    void broadcast(const std::string& data) const;
    void wait_message_from(Client client, size_t buffer_size, THREAD on_message_thread_run);
    void close() const;
};

#endif //SERVER_SOCKET_H

#include "ServerSocket.h"
#include <thread>
#include <cstring>


bool Client::operator==(const Client& client) const {
    return client.socket_fd == this->socket_fd;
}
bool Client::operator!=(const Client& client) const {
    return client.socket_fd != this->socket_fd;
}

/* private */

/* public */
ServerSocket::ServerSocket(uint16_t port, uint8_t backlog_queue_size, uint8_t max_connections) {
    this->max_connections = max_connections;
    this->socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0) {
        /* todo can't create socket */
    }

    bzero((char *) &address, sizeof(address));
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);

    int one = 1;
    setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &one, (socklen_t)sizeof one);

    if (::bind(this->socket_fd, (sockaddr *) &this->address, sizeof(this->address)) < 0) {
        /* todo can't bind socket */
    }
    ::listen(this->socket_fd, backlog_queue_size);
}
ServerSocket::~ServerSocket() {
    this->close();
}

void ServerSocket::wait_connection(THREAD on_connect_thread_run) {
    this->ON_CONNECT_THREAD_RUN = on_connect_thread_run;
    std::thread connection_thread([this]() {
        while (this->max_connections == 0 || this->clients.size() < this->max_connections) { /* if max connections == 0, listen unlimited connections */
            Client client{};
            socklen_t client_len = sizeof(client.address);
            client.socket_fd = ::accept(this->socket_fd, (sockaddr *) &client.address, &client_len);
            mutex.lock();
            this->clients.push_back(client);
            mutex.unlock();
            this->on_connect_callback(client);
        }
    });
    switch (this->ON_CONNECT_THREAD_RUN) {
        case THREAD::JOIN:
            connection_thread.join();
        case THREAD::DETACH:
            connection_thread.detach();
    }
}
void ServerSocket::wait_message_from(Client client, size_t buffer_size, THREAD on_message_thread_run) {
    this->ON_MESSAGE_THREAD_RUN = on_message_thread_run;
    char *buffer = new char[buffer_size];
    std::thread message_thread([this, client, buffer, buffer_size]() {
        while (true) {
            bzero(buffer, buffer_size);
            int64_t read = ::read(client.socket_fd, buffer, buffer_size);
            if (read > 0) {
                this->on_message_from_callback(buffer, client);
            } else if (read == 0) { /* when receiving 0, it means that client is disconnected. no need to listen messages from that client */
                this->on_disconnect_callback(client);
                mutex.lock();
                this->clients.remove(client);
                if (this->clients.size() == this->max_connections - 1) { /* when there is available place for new connection, restart listening new connections */
                    this->wait_connection(this->ON_CONNECT_THREAD_RUN);
                }
                mutex.unlock();
                break;
            }
        }
    });
    switch (this->ON_MESSAGE_THREAD_RUN) {
        case THREAD::JOIN:
            message_thread.join();
        case THREAD::DETACH:
            message_thread.detach();
    }
}

void ServerSocket::broadcast(const std::string& data) const {
    for (Client client: this->clients) {
        std::thread([client, data]() {
            ::send(client.socket_fd, data.c_str(), data.size(), 0);
        }).detach();
    }
}

void ServerSocket::close() const {
    shutdown(this->socket_fd, SHUT_RDWR);
    ::close(this->socket_fd);
    for (Client client: this->clients) {
        shutdown(client.socket_fd, SHUT_RDWR);
        ::close(client.socket_fd);
    }
}
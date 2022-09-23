#include "ServerSocket.h"
#include "../exception/socket_exception.h"
#include <thread>
#include <cstring>
#include <iostream>

/* private */
/* public */
ServerSocket::ServerSocket(uint8_t max_connections) {
    this->socket_fd = 0;
    this->max_connections = max_connections;
}
ServerSocket::~ServerSocket() {
    this->close();
}
void ServerSocket::create() {
    this->socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0) {
        throw socket_creation_exception();
    }
}
void ServerSocket::bind(uint16_t port) {
    bzero((char *) &address, sizeof(address));
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);

    int one = 1;
    setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &one, (socklen_t)sizeof one);

    if (::bind(this->socket_fd, (sockaddr *) &this->address, sizeof(this->address)) < 0) {
        throw socket_bind_exception(port);
    }
}
void ServerSocket::listen(uint8_t backlog_queue_size) const {
    ::listen(this->socket_fd, backlog_queue_size);
}

std::thread ServerSocket::wait_connection() {
    return std::thread([this]() {
        while (this->max_connections == 0 || this->clients.size() < this->max_connections) { /* if max connections == 0, listen unlimited connections */
            Client client{};
            socklen_t client_len = sizeof(client.address);
            client.socket_fd = ::accept(this->socket_fd, (sockaddr *) &client.address, &client_len);
            this->mutex.lock();
            this->clients.push_back(client);
            this->mutex.unlock();
            this->on_connect(client);
        }
    });
}
std::thread ServerSocket::wait_message_from(Client client, size_t buffer_size) {
    char *buffer = new char[buffer_size];
    return std::thread([this, client, buffer, buffer_size]() {
        while (true) {
            bzero(buffer, buffer_size);
            int64_t read = ::read(client.socket_fd, buffer, buffer_size);
            if (read > 0) {
                this->on_message_from(buffer, client);
            } else if (read == 0) { /* when receiving 0, it means that client is disconnected. no need to listen messages from that client */
                this->mutex.lock();
                this->clients.remove(client);
                this->mutex.unlock();
                this->on_disconnect(client);
                break;
            }
        }
    });
}

void ServerSocket::broadcast(const std::string& data) const {
    for (Client client: this->clients) {
        std::thread([client, data]() {
            ::send(client.socket_fd, data.c_str(), data.size(), 0);
        }).detach();
    }
}
void ServerSocket::send_to(Client client, const std::string &data) const {
    ::send(client.socket_fd, data.c_str(), data.size(), 0);
}

void ServerSocket::close() const {
    shutdown(this->socket_fd, SHUT_RDWR);
    ::close(this->socket_fd);
    for (Client client: this->clients) {
        shutdown(client.socket_fd, SHUT_RDWR);
        ::close(client.socket_fd);
    }
}
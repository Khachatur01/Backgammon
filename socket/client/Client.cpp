#include <thread>
#include <cstring>
#include <iostream>
#include "Client.h"
#include "../exception/socket_exception.h"

/* private */

/* public */
Socket::Client::~Client() {
    this->close();
}

void Socket::Client::create() {
    this->socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0) {
        throw socket_creation_exception();
    }
}
void Socket::Client::init_host(const std::string& hostname, uint16_t port) {
    this->server = gethostbyname(hostname.c_str());
    if (this->server == nullptr) {
        throw server_init_exception();
    }

    bzero((char *) &server_address, sizeof(server_address));
    this->server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&server_address.sin_addr.s_addr,
          server->h_length);

    this->server_address.sin_port = htons(port);

    int one = 1;
    setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &one, (socklen_t)sizeof one);
}

bool Socket::Client::connect() {
    if (::connect(this->socket_fd, (sockaddr *) &this->server_address, sizeof(this->server_address)) < 0) {
        return false;
    }
    return true;
}
void Socket::Client::send(const std::string& data) const {
    ::send(this->socket_fd, data.c_str(), data.size(), 0);
}

std::thread Socket::Client::wait_message(size_t buffer_size) {
    return std::thread([this, buffer_size]() {
        char *buffer = new char[buffer_size];
        while (true) {
            bzero(buffer, buffer_size);
            int64_t read = ::read(this->socket_fd, buffer, buffer_size);
            if (read > 0) {
                this->on_message(buffer);
            } else if (read == 0) { /* when receiving 0, it means that socket is disconnected */
                this->on_disconnect();
                break;
            }
        }
        delete[] buffer;
    });
}
void Socket::Client::close() const {
    ::shutdown(this->socket_fd, SHUT_RDWR);
    ::close(this->socket_fd);
}

#include <thread>
#include <cstring>
#include <iostream>
#include "ClientSocket.h"

/* private */

/* public */
ClientSocket::ClientSocket(const std::string& hostname, uint16_t port) {
    this->socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0) {
        /* todo can't create socket */
    }

    this->server = gethostbyname(hostname.c_str());
    if (this->server == nullptr) {
        /* todo no such host */
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
ClientSocket::~ClientSocket() {
    this->close();
}

bool ClientSocket::connect() {
    if (::connect(this->socket_fd, (sockaddr *) &this->server_address, sizeof(this->server_address)) < 0) {
        return false;
    }
    return true;
}
bool ClientSocket::send(const std::string& data) const {
    if (::write(this->socket_fd, data.c_str(), data.size()) < 0) {
        return false;
    }
    return true;
}

void ClientSocket::wait_message(size_t buffer_size, THREAD on_message_thread_run) {
    this->ON_MESSAGE_THREAD_RUN = on_message_thread_run;
    char *buffer = new char[buffer_size];

    std::thread message_thread([this, buffer, buffer_size]() {
        while (true) {
            bzero(buffer, buffer_size);
            int64_t read = ::read(this->socket_fd, buffer, buffer_size);
            if (read > 0) {
                this->on_message_callback(buffer);
            } else if (read == 0) { /* when receiving 0, it means that socket is disconnected */
                this->on_disconnect_callback();
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
void ClientSocket::close() const {
    ::shutdown(this->socket_fd, SHUT_RDWR);
    ::close(this->socket_fd);
}

#include <cstring>
#include <cstdint>
#include "socket_exception.h"

[[maybe_unused]] const char* socket_creation_exception::what() {
    return (char*)"Can't create socket";
}

socket_bind_exception::socket_bind_exception(uint16_t port) {
    this->port = port;
}
[[maybe_unused]] const char* socket_bind_exception::what() {
    char* message = new char[40];
    strcpy(message, "Can't bind socket on port ");

    char port_str[4];
    uint16_t port_cpy = this->port;
    for (int8_t i = 3; i >= 0; --i) {
        port_str[i] = char(port_cpy % 10 + '0');
        port_cpy /= 10;
    }

    strcat(message, port_str);
    return message;
}
[[maybe_unused]] uint16_t socket_bind_exception::get_port() const {
    return this->port;
}

[[maybe_unused]] const char* server_init_exception::what() {
    return (char*)"Can't init server socket";
}

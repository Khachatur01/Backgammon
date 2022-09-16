#include <iostream>
#include "socket/server//ServerSocket.h"


int main() {
    ServerSocket serverSocket(5555, 5, 2);

    std::cout << "ServerSocket initialized, listening on port 5555\n";

    serverSocket.on_connect_callback = [&](Client client) {
        std::cout << client.socket_fd << " is connected\n";
        serverSocket.wait_message_from(client, 200, THREAD::DETACH);
    };
    serverSocket.on_disconnect_callback = [&](Client client) {
        std::cout << client.socket_fd << " is disconnected\n";
    };

    serverSocket.on_message_from_callback = [&](const std::string &data, Client from) {
        std::cout << "From: " << from.socket_fd << " - Received message: " << data << std::endl;
        serverSocket.broadcast("hi everyone");
    };

    serverSocket.wait_connection(THREAD::JOIN);

    int i;
    std::cin >> i;

    return 0;
}
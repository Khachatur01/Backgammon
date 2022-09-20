#include <iostream>
#include "socket/server/ServerSocket.h"
#include "socket/exception/socket_exception.h"

int main() {
    ServerSocket serverSocket(2);
    try {
        serverSocket.create();
    } catch (socket_creation_exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }

    try {
        serverSocket.bind(5555);
    } catch (socket_bind_exception &e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
    serverSocket.listen(2);

    std::cout << "ServerSocket initialized, listening on port 5555\n";

    serverSocket.on_connect = [&](Client client) {
        std::cout << "\n" << client.socket_fd << " is connected\n";
        serverSocket.wait_message_from(client, 200).detach();
    };
    serverSocket.on_disconnect = [&](Client client) {
        std::cout << "\n" << client.socket_fd << " is disconnected\n";
    };
    serverSocket.on_message_from = [&](const std::string &data, Client from) {
        std::cout << "\n" << "From: " << from.socket_fd << " - Received message: " << data << "\n";
        serverSocket.broadcast("From: " + std::to_string(from.socket_fd) + " - Received message: " + data);
        serverSocket.send_to(from, "message sent from you :)");
    };

    serverSocket.wait_connection().join();

    int i;
    std::cin >> i;

    return 0;
}
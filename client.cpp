#include <iostream>
#include "socket/client/ClientSocket.h"

int main() {
    std::string data;
    ClientSocket clientSocket;
    clientSocket.create();
    clientSocket.init_host("localhost", 5555);

    std::cout << "Connecting...\n";
    if (clientSocket.connect()) {
        std::cout << "Connected\n";

        clientSocket.on_message = [](const std::string &data) {
            std::cout << "\nMessage from socket: " << data << "\n";
        };
        clientSocket.on_disconnect = []() {
            std::cout << "\nDisconnect" << "\n";
        };

        clientSocket.wait_message(200).detach();

        while (true) {
            std::cin >> data;
            if (data == "q") {
                break;
            }
            if (clientSocket.send(data)) {
                std::cout << "\nSent!\n";
            }
        }
    }


    return 0;
}
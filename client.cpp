#include <iostream>
#include "socket/client/ClientSocket.h"

int main() {
    std::string data;
    ClientSocket clientSocket("localhost", 5555);
    std::cout << "Connecting...\n";
    if (clientSocket.connect()) {
        std::cout << "Connected\n";

        clientSocket.on_message_callback = [](const std::string &data) {
            std::cout << "Message from socket: " << data << std::endl;
        };

        clientSocket.wait_message(200, THREAD::DETACH);

        while (true) {
            std::cin >> data;
            if (data == "q") {
                break;
            }
            if (clientSocket.send(data)) {
                std::cout << "Sent!\n";
            }
        }
    }


    return 0;
}
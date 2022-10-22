#include <iostream>
#include "game/mode/network/server/BackgammonServer.h"
#include "socket/exception/socket_exception.h"

int main() {
    uint64_t max_rooms;
    uint16_t port;

    system("clear");

    std::cout << "\n"
                 "██████╗  █████╗  █████╗ ██╗  ██╗ ██████╗  █████╗ ███╗   ███╗███╗   ███╗ █████╗ ███╗  ██╗\n"
                 "██╔══██╗██╔══██╗██╔══██╗██║ ██╔╝██╔════╝ ██╔══██╗████╗ ████║████╗ ████║██╔══██╗████╗ ██║\n"
                 "██████╦╝███████║██║  ╚═╝█████═╝ ██║  ██╗ ███████║██╔████╔██║██╔████╔██║██║  ██║██╔██╗██║\n"
                 "██╔══██╗██╔══██║██║  ██╗██╔═██╗ ██║  ╚██╗██╔══██║██║╚██╔╝██║██║╚██╔╝██║██║  ██║██║╚████║\n"
                 "██████╦╝██║  ██║╚█████╔╝██║ ╚██╗╚██████╔╝██║  ██║██║ ╚═╝ ██║██║ ╚═╝ ██║╚█████╔╝██║ ╚███║\n"
                 "╚═════╝ ╚═╝  ╚═╝ ╚════╝ ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚═╝     ╚═╝ ╚════╝ ╚═╝  ╚══╝\n"
                 "\n"
                 " ██████╗███████╗██████╗ ██╗   ██╗███████╗██████╗ \n"
                 "██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗\n"
                 "╚█████╗ █████╗  ██████╔╝╚██╗ ██╔╝█████╗  ██████╔╝\n"
                 " ╚═══██╗██╔══╝  ██╔══██╗ ╚████╔╝ ██╔══╝  ██╔══██╗\n"
                 "██████╔╝███████╗██║  ██║  ╚██╔╝  ███████╗██║  ██║\n"
                 "╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝\n\n\n";

    std::cout << "Max rooms(0 for infinity): ";
    std::cin >> max_rooms;
    port_input:
    std::cout << "Run server on port: ";
    std::cin >> port;

    BackgammonServer backgammonServer;

    try {
        backgammonServer.run(port, max_rooms).join();
    } catch (socket_creation_exception &e) {
        std::cout << e.what() << std::endl;
    } catch (socket_bind_exception &e) {
        std::cout << e.what() << std::endl;
        goto port_input;
    }
}
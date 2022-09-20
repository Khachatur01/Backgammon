#include <iostream>
#include "server/BackgammonServer.h"
#include "../../../socket/exception/socket_exception.h"
#include "client/BackgammonClient.h"

void create_room() {
    uint16_t port;
    std::cout << "Run server on port: ";
    std::cin >> port;
    BackgammonServer server;
    try {
        server.run(port); /* wait until game over */
    } catch (socket_creation_exception &e) {
        std::cout << e.what() << std::endl;
    } catch (socket_bind_exception &e) {
        std::cout << e.what() << std::endl;
    }
}
void connect_to_room() {
    std::string host;
    uint16_t port;
    std::cout << "Server host: ";
    std::cin >> host;
    std::cout << "Server port: ";
    std::cin >> port;

    BackgammonClient client;
    client.connect(host, port); /* wait until game over */
}

void network_mode() {
    Backgammon::clear_screen();

    uint8_t room_type;

    do {
        std::cout << "Network mode.\n";
        std::cout << "<) Back\n";
        std::cout << "a) Create a room\n";
        std::cout << "b) Connect to room\n";

        do {
            std::cin >> room_type;
        } while (room_type != '<' && room_type < 'a' || room_type > 'b');

        if (room_type == '<') {
            break;
        }

        switch (room_type) {
            case 'a':
                Backgammon::clear_screen();
                create_room(); /* create room, connect to that room and wait for game over */
                break;
            case 'b':
                Backgammon::clear_screen();
                connect_to_room();
                break;
            default:
                break;
        }
    } while (true);
}
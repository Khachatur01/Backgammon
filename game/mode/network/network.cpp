#include <iostream>
#include "client/BackgammonClient.h"
#include "../../../socket/exception/socket_exception.h"

void choose_server(BackgammonClient& client) {
    std::string host;
    uint16_t port;

    choose_server:
    std::cout << "Host: ";
    std::cin >> host;
    std::cout << "Port: ";
    std::cin >> port;

    try {
        if (!client.set_server(host, port)) {
            Backgammon::clear_screen();
            std::cout << "Can't connect to host... Try again\n";
            goto choose_server;
        }
    } catch (socket_creation_exception &e) {
    } catch (server_init_exception &e) {
        Backgammon::clear_screen();
        std::cout << "Can't connect to host... Try again\n";
        goto choose_server;
    }
}
void create_room(BackgammonClient& client) {
    std::string room;
    std::string password;

    std::cout << "Room: ";
    std::cin >> room;
    std::cout << "Password: ";
    std::cin >> password;

    Backgammon::clear_screen();
    client.create_room_and_connect(room, password).join();
}
void connect_to_room(BackgammonClient& client) {
    std::string room;
    std::string password;

    std::cout << "Room: ";
    std::cin >> room;
    std::cout << "Password: ";
    std::cin >> password;

    client.connect_room(room, password).join();
}

void network_mode() {
    BackgammonClient client;

    uint8_t room_type;

    do {
        Backgammon::clear_screen();
        std::cout << "Network mode.\n";
        if (!client.get_host().empty()) {
            std::cout << "Host -> " << client.get_host() << ':' << client.get_port() << '\n';
        } else {
            std::cout << "Host not selected...\n";
        }

        std::cout << "<) Back\n";
        std::cout << "a) Choose server\n";
        std::cout << "b) Create a room\n";
        std::cout << "c) Connect to room\n";

        do {
            std::cin >> room_type;
        } while (room_type != '<' && room_type < 'a' || room_type > 'c');

        if (room_type == '<') {
            break;
        }

        Backgammon::clear_screen();
        switch (room_type) {
            case 'a':
                choose_server(client);
                break;
            case 'b':
                if (!client.get_host().empty()) {
                    create_room(client);
                }
                break;
            case 'c':
                if (!client.get_host().empty()) {
                    connect_to_room(client);
                }
                break;
            default:
                break;
        }
    } while (true);
}
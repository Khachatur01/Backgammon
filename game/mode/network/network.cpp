#include <iostream>
#include "client/BackgammonClient.h"
#include "../../../socket/exception/socket_exception.h"
#include <thread>
#include <chrono>

void choose_server(BackgammonClient& client) {
    std::string host;
    uint16_t port;

    choose_server:
    std::cout << "Host: ";
    std::cin >> host;
    std::cout << "Port: ";
    std::cin >> port;

    try {
        client.set_server(host, port);
    } catch (socket_creation_exception &e) {
        std::cout << e.what() << std::endl;
    } catch (socket_bind_exception &e) {
        std::cout << e.what() << std::endl;
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
        if (!client.get_host().empty()) {
            std::cout << "Host -> " << client.get_host() << ':' << client.get_port() << '\n';
        }
        std::cout << "Network mode.\n";
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

        switch (room_type) {
            case 'a':
                Backgammon::clear_screen();
                choose_server(client);
                break;
            case 'b':
                Backgammon::clear_screen();
                create_room(client); /* create room, set_server to that room and wait for game over */
                break;
            case 'c':
                Backgammon::clear_screen();
                connect_to_room(client);
                break;
            default:
                break;
        }
    } while (true);
}
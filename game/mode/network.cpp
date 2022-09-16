#ifndef IO_STREAM
#include <iostream>
#define IO_STREAM
#endif

#include "../backgammon/backgammon.h"

void network_mode() {
    Backgammon::clear_screen();

    char room_type;
    std::string host = "localhost";
    uint16_t port;

    std::cout << "Network mode.\n";
    std::cout << "a) Create a room\n";
    std::cout << "b) Connect to room\n";

    do {
        std::cin >> room_type;
    } while (room_type < 'a' || room_type > 'c');

    switch (room_type) {
        case 'a':
            
            break;
        case 'b':
            
            break;
        default:
            break;
    }
}
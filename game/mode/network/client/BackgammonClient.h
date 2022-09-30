#ifndef BACKGAMMON_CLIENT_H
#define BACKGAMMON_CLIENT_H

#include "../../../backgammon/backgammon.h"
#include "../../../../socket/client/Client.h"
#include "../event/Event.h"

class BackgammonClient {
private:
    Backgammon* backgammon;
    Socket::Client* clientSocket;
    Player_t me;

    std::string host;
    uint16_t port;
    std::string room_name;
    std::string room_password;

    void on_play();
    void on_event(const std::string& message);
    void on_event(event::Event* event);
public:
    BackgammonClient();
    bool set_server(const std::basic_string<char> &hostname, uint16_t port_number);

    [[maybe_unused]] void create_room(const std::string& room, const std::string& password);
    [[maybe_unused]] std::thread create_room_and_connect(const std::string &room, const std::string &password);
    std::thread connect_room(const std::string& room, const std::string& password);

    [[nodiscard]] std::string get_host() const;
    [[nodiscard]] uint16_t get_port() const;
};

#endif //BACKGAMMON_CLIENT_H

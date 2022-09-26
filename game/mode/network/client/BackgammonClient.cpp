#include <iostream>
#include "BackgammonClient.h"

/* private */
void BackgammonClient::on_play() {
    dices_t dices = this->backgammon->throw_dice();
    this->clientSocket->send(event::ThrowDices(this->room_name, this->room_password, dices.first, dices.second).to_string());

    bool valid_peace;
    bool valid_move;
    uint8_t to;
    uint8_t pip;

    game_over_checking_loop:
    while (backgammon->get_winner() == nullptr) {
        do {
            backgammon->render(); /* to clear unnecessary messages */
            std::cout << "Take peace(0 to cancel all moves. 1 to commit): ";
            std::cin >> pip;
            if (pip == '0') {
                backgammon->cancel_moves();
                this->clientSocket->send(event::CancelMoves(this->room_name, this->room_password).to_string());
                continue;
            } else if (pip == '1' && backgammon->commit_moves()) {
                this->clientSocket->send(event::CommitMoves(this->room_name, this->room_password).to_string());
                goto finish_playing;
            }
            valid_peace = backgammon->take_peace(pip);
            if (valid_peace) {
                this->clientSocket->send(event::TakePeace(this->room_name, this->room_password, pip).to_string());
            }
        } while (!valid_peace);

        do {
            backgammon->render(); /* to clear unnecessary messages */
            std::cout << "Move to(0 to release peace): ";
            std::cin >> to;
            if (to == '0') {
                backgammon->release_peace();
                this->clientSocket->send(event::ReleasePeace(this->room_name, this->room_password).to_string());
                goto game_over_checking_loop;
            }
            valid_move = backgammon->move_to(to);
            if (valid_move) {
                this->clientSocket->send(event::MoveTo(this->room_name, this->room_password, to).to_string());
            }
        } while (!valid_move);
    }

    /* game over */
    this->clientSocket->send(event::GameOver(this->room_name, this->room_password, backgammon->get_winner()->TYPE).to_string());

    finish_playing:
    return;
}

void BackgammonClient::on_event(const std::string& event) {
    this->on_event(event::parse_message(event));
}
void BackgammonClient::on_event(event::Event* event) {
    switch (event->id) {
        case ROOM_EXISTS: {
            std::cout << "Room '" << event->room << "' exits\n";
            this->clientSocket->close();
            break;
        }
        case INCORRECT_PASSWORD: {
            std::cout << "'" << event->password << "' password for '" << event->password << "' room is incorrect\n";
            this->clientSocket->close();
            break;
        }
        case Event_t::SET_MYSELF: {
            auto* set_myself = dynamic_cast<event::SetMyself*>(event);
            this->me = set_myself->me;
            std::cout << "Connected to room '" << event->room << "'\n";
            std::cout << "Wait opponent to connect...'\n";
            break;
        }
        case Event_t::START: {
            auto* start = dynamic_cast<event::Start*>(event);
            this->backgammon = new Backgammon(false, this->me);

            Backgammon::clear_screen();
            this->backgammon->start(start->starter);
            if (start->starter == this->me) {
                this->on_play();
            }
            std::cout << "Wait opponent to play...\n";

            break;
        }
        case Event_t::THROW_DICES: {
            auto* throw_dices = dynamic_cast<event::ThrowDices*>(event);
            auto force_dices = std::make_pair(throw_dices->first, throw_dices->second);
            this->backgammon->throw_dice(true, &force_dices);
            break;
        }
        case Event_t::TAKE_PEACE: {
            auto* take_peace = dynamic_cast<event::TakePeace*>(event);
            this->backgammon->take_peace( take_peace->pip, true);
            break;
        }
        case Event_t::MOVE_TO: {
            auto* move_to = dynamic_cast<event::MoveTo*>(event);
            this->backgammon->move_to(move_to->pip, true);
            break;
        }
        case Event_t::CANCEL_MOVES: {
            this->backgammon->cancel_moves();
            break;
        }
        case Event_t::RELEASE_PEACE: {
            this->backgammon->release_peace();
            break;
        }
        case Event_t::COMMIT_MOVES: {
            this->backgammon->commit_moves();
            this->on_play();
            std::cout << "Wait opponent to play...\n";
            break;
        }
        default:
            break;
    }
}

/* public */
BackgammonClient::BackgammonClient(): me(Player_t::SWITCH) {
    this->backgammon = nullptr;
    this->port = 5555;

    this->clientSocket = new Socket::Client();
    this->clientSocket->on_message = [&](const std::string &data) {
        this->on_event(data);
    };
    this->clientSocket->on_disconnect = []() {
        std::cout << "Server disconnected...\n";
    };

    this->clientSocket->create();
}

void BackgammonClient::set_server(const std::basic_string<char> &hostname, uint16_t port_number) {
    this->host = hostname;
    this->port = port_number;
    this->clientSocket->init_host(hostname, port_number);
    this->clientSocket->connect();
}

void BackgammonClient::create_room(const std::string& room, const std::string& password) {
    this->clientSocket->send(event::CreateRoom(room, password).to_string());
}

std::thread BackgammonClient::connect_room(const std::string& room, const std::string& password) {
    this->clientSocket->send(event::ConnectRoom(room, password).to_string());
    this->room_name = room;
    this->room_password = password;
    return this->clientSocket->wait_message(200);
}

std::thread BackgammonClient::create_room_and_connect(const std::string &room, const std::string &password) {
    this->clientSocket->send(event::CreateRoomAndConnect(room, password).to_string());
    this->room_name = room;
    this->room_password = password;
    return this->clientSocket->wait_message(200);
}

std::string BackgammonClient::get_host() const {
    return this->host;
}
uint16_t BackgammonClient::get_port() const {
    return this->port;
}
#ifndef EVENT
#define EVENT

#include "Event.h"

/* Event */
event::Event::Event(Event_t id): id(id) {}
event::Event::Event(Event_t id, std::string room, std::string password): id(id), room(std::move(room)), password(std::move(password)) {}

std::string event::Event::to_string() {
    return std::to_string(id) + sep + this->room + sep + this->password;
}
void event::Event::parse(std::vector<std::string> fields) {
    this->room = fields[1];
    this->password = fields[2];
}
event::Event* event::parse_message(const std::string& data) {
    Event* event = nullptr;

    std::vector<std::string> event_list = event::split(data, Event::sep);
    auto id = static_cast<Event_t>(std::stoi(event_list[0]));
    switch (id) {
        case Event_t::CREATE_ROOM: {
            event = new CreateRoom();
            break;
        }
        case Event_t::CONNECT_ROOM: {
            event = new ConnectRoom();
            break;
        }
        case Event_t::CREATE_ROOM_AND_CONNECT: {
            event = new CreateRoomAndConnect();
            break;
        }
        case Event_t::SET_MYSELF: {
            event = new SetMyself();
            break;
        }
        case Event_t::START: {
            event = new Start();
            break;
        }
        case Event_t::THROW_DICES: {
            event = new ThrowDices();
            break;
        }
        case Event_t::TAKE_PEACE: {
            event = new TakePeace();
            break;
        }
        case Event_t::MOVE_TO: {
            event = new MoveTo();
            break;
        }
        case Event_t::CANCEL_MOVES: {
            event = new CancelMoves();
            break;
        }
        case Event_t::RELEASE_PEACE: {
            event = new ReleasePeace();
            break;
        }
        case Event_t::COMMIT_MOVES: {
            event = new CommitMoves();
            break;
        }
    }

    if (event) {
        event->parse(event_list);
    }
    return event;
}

std::vector<std::string> event::split(const std::string& string, char separator) {
    std::stringstream string_stream(string);
    std::string segment;
    std::vector<std::string> list;
    while (std::getline(string_stream, segment, separator)) {
        list.push_back(segment);
    }
    if (string.ends_with(event::Event::sep)) {
        list.emplace_back("");
    }
    return list;
}

/* CreateRoom */
event::CreateRoom::CreateRoom() : event::Event(Event_t::CREATE_ROOM) {}
event::CreateRoom::CreateRoom(const std::string& room, const std::string& password)
    : event::Event(Event_t::CREATE_ROOM, room, password) {
}

/* ConnectRoom */
event::ConnectRoom::ConnectRoom() : event::Event(Event_t::CONNECT_ROOM) {}
event::ConnectRoom::ConnectRoom(const std::string& room, const std::string& password)
    : event::Event(Event_t::CONNECT_ROOM, room, password) {
}

/* CreateRoomAndConnect */
event::CreateRoomAndConnect::CreateRoomAndConnect() : event::Event(Event_t::CREATE_ROOM_AND_CONNECT) {}
event::CreateRoomAndConnect::CreateRoomAndConnect(const std::string& room, const std::string& password)
    : event::Event(Event_t::CREATE_ROOM_AND_CONNECT, room, password) {}

/* GameOver */
event::GameOver::GameOver() : event::Event(Event_t::GAME_OVER) {}
event::GameOver::GameOver(const std::string& room, const std::string& password, Player_t winner)
    : event::Event(Event_t::GAME_OVER, room, password), winner(winner) {}
std::string event::GameOver::to_string() {
    return event::Event::to_string() + sep + std::to_string(this->winner);
}
void event::GameOver::parse(std::vector<std::string> fields) {
    event::Event::parse(fields);
    this->winner = static_cast<Player_t>(std::stoi(fields[3]));
}

/* RoomExist */
event::RoomExists::RoomExists() : event::Event(Event_t::ROOM_EXISTS) {}
event::RoomExists::RoomExists(const std::string& room, const std::string& password)
        : event::Event(Event_t::ROOM_EXISTS, room, password) {}

/* IncorrectPassword */
event::IncorrectPassword::IncorrectPassword() : event::Event(Event_t::INCORRECT_PASSWORD) {}
event::IncorrectPassword::IncorrectPassword(const std::string& room, const std::string& password)
        : event::Event(Event_t::INCORRECT_PASSWORD, room, password) {}

/* SetMyself */
event::SetMyself::SetMyself() : event::Event(Event_t::SET_MYSELF) {}
event::SetMyself::SetMyself(const std::string& room, const std::string& password, Player_t me)
    : event::Event(Event_t::SET_MYSELF, room, password), me(me) {}
std::string event::SetMyself::to_string() {
    return event::Event::to_string() + sep + std::to_string(this->me);
}
void event::SetMyself::parse(std::vector<std::string> fields) {
    event::Event::parse(fields);
    this->me = static_cast<Player_t>(std::stoi(fields[3]));
}

/* Start */
event::Start::Start() : event::Event(Event_t::START) {}
event::Start::Start(const std::string& room, const std::string& password, Player_t starter)
    : event::Event(Event_t::START, room, password), starter(starter) {}
std::string event::Start::to_string() {
    return event::Event::to_string() + sep + std::to_string(this->starter);
}
void event::Start::parse(std::vector<std::string> fields) {
    event::Event::parse(fields);
    this->starter = static_cast<Player_t>(std::stoi(fields[3]));
}

/* ThrowDices */
event::ThrowDices::ThrowDices() : event::Event(Event_t::THROW_DICES) {}
event::ThrowDices::ThrowDices(const std::string& room, const std::string& password, uint8_t first, uint8_t second)
    : event::Event(Event_t::THROW_DICES, room, password), first(first), second(second) {}
std::string event::ThrowDices::to_string() {
    return event::Event::to_string() + sep + std::to_string(first) + sep + std::to_string(second);
}
void event::ThrowDices::parse(std::vector<std::string> fields) {
    event::Event::parse(fields);
    this->first = std::stoi(fields[3]);
    this->second = std::stoi(fields[4]);
}

/* TakePeace */
event::TakePeace::TakePeace() : event::Event(Event_t::TAKE_PEACE) {}
event::TakePeace::TakePeace(const std::string& room, const std::string& password, uint8_t pip)
    : event::Event(Event_t::TAKE_PEACE, room, password), pip(pip) {}
std::string event::TakePeace::to_string() {
    return event::Event::to_string() + sep + std::to_string(pip);
}
void event::TakePeace::parse(std::vector<std::string> fields) {
    event::Event::parse(fields);
    this->pip = std::stoi(fields[3]);
}

/* MoveTo */
event::MoveTo::MoveTo() : event::Event(Event_t::MOVE_TO) {}
event::MoveTo::MoveTo(const std::string& room, const std::string& password, uint8_t pip)
    : event::Event(Event_t::MOVE_TO, room, password), pip(pip) {}
std::string event::MoveTo::to_string() {
    return event::Event::to_string() + sep + std::to_string(pip);
}
void event::MoveTo::parse(std::vector<std::string> fields) {
    event::Event::parse(fields);
    this->pip = std::stoi(fields[3]);
}

/* CancelMoves */
event::CancelMoves::CancelMoves() : event::Event(Event_t::CANCEL_MOVES) {}
event::CancelMoves::CancelMoves(const std::string& room, const std::string& password)
    : event::Event(Event_t::CANCEL_MOVES, room, password) {}

/* ReleasePeace */
event::ReleasePeace::ReleasePeace() : event::Event(Event_t::RELEASE_PEACE) {}
event::ReleasePeace::ReleasePeace(const std::string& room, const std::string& password)
    : event::Event(Event_t::RELEASE_PEACE, room, password) {}

/* CommitMoves */
event::CommitMoves::CommitMoves() : event::Event(Event_t::COMMIT_MOVES) {}
event::CommitMoves::CommitMoves(const std::string& room, const std::string& password)
    : event::Event(Event_t::COMMIT_MOVES, room, password) {}


#endif //EVENT
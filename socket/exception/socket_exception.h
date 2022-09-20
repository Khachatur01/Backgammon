#ifndef BACKGAMMON_SOCKET_EXCEPTION_H
#define BACKGAMMON_SOCKET_EXCEPTION_H

class server_init_exception/* : public std::exception*/ {
public:
    virtual const char* what();
};

class socket_creation_exception/* : public std::exception*/ {
public:
    virtual const char* what();
};

class socket_bind_exception/* : public std::exception*/ {
private:
    uint16_t port;
public:
    explicit socket_bind_exception(uint16_t port);
    virtual const char* what();
    [[nodiscard]] uint16_t get_port() const;
};

#endif //BACKGAMMON_SOCKET_EXCEPTION_H

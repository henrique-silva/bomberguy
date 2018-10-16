#ifndef REMOTE_VIEW_HPP
#define REMOTE_VIEW_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fstream>
#include <string>
#include "map.hpp"

class Spectator {
private:
    int socket_fd;
    int connection_fd;
    struct sockaddr_in myself_addr, client_addr;
    socklen_t client_size;

#ifdef LOG_FILE
    std::ofstream out_fs;
#endif

    Map *map;

public:
    Spectator(Map *map);
    ~Spectator();
    void connect();
    void update();
};

#endif

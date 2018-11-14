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
#include "player.hpp"

#define LOG_FILE

class Spectator {
private:
    int socket_fd;
    int connection_fd;
    struct sockaddr_in myself_addr, client_addr;
    socklen_t client_size;

#ifdef LOG_FILE
    std::ofstream out_fs;
#endif

    Player *player;
    Map *map;

public:
    Spectator(Map *map, Player *player);
    ~Spectator();
    void connect();
    void update();
    void send_map_info();
    void send_player_info();
    void send_config_data();
    void send_sound_alert(std::string sound);
    Player* get_player();
};

#endif

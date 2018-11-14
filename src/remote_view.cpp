#include "remote_view.hpp"

Spectator::Spectator(Map *map, Player *player)
{
    this->map = map;
    this->player = player;
    this->connection_fd = player->get_socket_fd();
}

Player* Spectator::get_player()
{
    return this->player;
}

void Spectator::send_config_data()
{
    std::string buffer;

    /* Map size */
    buffer.append("X");
    buffer.append(std::to_string(this->map->get_size_x()));
    buffer.append(" Y");
    buffer.append(std::to_string(this->map->get_size_y()));
    buffer.append(" P");
    buffer.append(std::to_string(this->player->get_id()));
    buffer.append(" \n");

    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);

    /* Player info */
    this->send_player_info();
}

void Spectator::send_player_info()
{
    std::string buffer;

    buffer.append("L");
    buffer.append(std::to_string(this->player->get_lives()));
    buffer.append(" S");
    buffer.append(std::to_string(this->player->get_score()));
    buffer.append(" B");
    buffer.append(std::to_string(this->player->get_bomb_count()));
    buffer.append(" \n");
    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);

}

void Spectator::send_map_info()
{
    int y, x;
    std::string buffer;

    buffer.append("M");
    for (y = 0; y < this->map->get_size_y(); y++) {
        for (x = 0; x < this->map->get_size_x(); x++) {
            buffer.append(this->map->get_flags(y, x).to_string());
            buffer.append(" ");
        }
    }
    buffer.append("#");

    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);
}

void Spectator::update()
{
    this->send_map_info();
    this->send_player_info();
}

void Spectator::send_sound_alert(std::string sound)
{
    std::string buffer;

    buffer.append("D");
    buffer.append(sound);

    buffer.append(" ");
    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);
}

void Spectator::send_winner_alert(Player *winner)
{
    std::string buffer;

    buffer.append("W");
    buffer.append(std::to_string(winner->get_id()));
    buffer.append(" \n");

    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);
}

void Spectator::send_gameover_alert()
{
    std::string buffer;

    buffer.append("E \n");

    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);
}


Spectator::~Spectator()
{
    std::string buffer("Q ");

    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);

    close(this->connection_fd);
    close(this->socket_fd);
}

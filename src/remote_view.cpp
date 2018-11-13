#include "remote_view.hpp"

Spectator::Spectator(Map *map, Player *player)
{
    this->map = map;
    this->player = player;

#ifdef LOG_FILE
    this->out_fs.open("map_log.txt", std::ios::out | std::ios::binary);
#endif

    this->client_size = (socklen_t)sizeof(this->client_addr);

    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    //printf("Socket criado\n");

    this->myself_addr.sin_family = AF_INET;
    this->myself_addr.sin_port = htons(3001);
    inet_aton("127.0.0.1", &(this->myself_addr.sin_addr));

    //printf("Tentando abrir porta 3001\n");
    if (bind(this->socket_fd, (struct sockaddr*)&(this->myself_addr), sizeof(this->myself_addr)) != 0) {
        //printf("Problemas ao abrir porta\n");
        return;
    }
    //printf("Abri porta 3001!\n");

    listen(this->socket_fd, 2);
    //printf("Estou ouvindo na porta 3001!\n");
}

void Spectator::connect()
{
    this->connection_fd = accept(this->socket_fd, (struct sockaddr*)&(this->client_addr), &(this->client_size));
}

void Spectator::send_config_data()
{
    std::string buffer;

    /* Map size */
    buffer.append("X");
    buffer.append(std::to_string(this->map->get_size_x()));
    buffer.append(" Y");
    buffer.append(std::to_string(this->map->get_size_y()));
    buffer.append(" \n");

#ifdef LOG_FILE
    this->out_fs.write(buffer.c_str(), buffer.size());
#endif
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
#ifdef LOG_FILE
    this->out_fs.write(buffer.c_str(), buffer.size());
#endif
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

#ifdef LOG_FILE
    this->out_fs.write(buffer.c_str(), buffer.size());
#endif
    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);
}

void Spectator::update()
{
    this->send_map_info();
    this->send_player_info();
}

Spectator::~Spectator()
{
    std::string buffer("Q");

#ifdef LOG_FILE
    this->out_fs.write(buffer.c_str(), buffer.size());
#endif
    send(this->connection_fd, buffer.c_str(), buffer.size(), 0);

    close(this->connection_fd);
    close(this->socket_fd);
#ifdef LOG_FILE
    this->out_fs.close();
#endif
}

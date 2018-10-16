#include "remote_view.hpp"

Spectator::Spectator(Map *map)
{
    this->map = map;
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

void Spectator::update()
{
    int y, x;
    std::vector<uint16_t> buffer;

    for (y = 0; y < this->map->get_size_y(); y++) {
        for (x = 0; x < this->map->get_size_x(); x++) {
            buffer.push_back(this->map->get_flags(y, x).to_ulong());
            buffer.push_back(0x00FE);
        }
        buffer.push_back(0x00FF);
    }
#ifdef LOG_FILE
    this->out_fs.write((char *)&buffer[0], buffer.size()*sizeof(uint16_t));
#endif
    send(this->connection_fd, (char *)&buffer[0], buffer.size()*sizeof(uint16_t), 0);
}

Spectator::~Spectator()
{
    close(this->connection_fd);
    close(this->socket_fd);
#ifdef LOG_FILE
    this->out_fs.close();
#endif
}

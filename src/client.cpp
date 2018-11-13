#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ncurses.h>

#include "screen.hpp"
#include "map.hpp"
#include "player.hpp"
#include "keyboard.hpp"

#define BUFFER_SIZE 1024

int read_until_stop(int socket_fd, char *buffer, char stop_char)
{
    int sz = 0;
    do {
        recv(socket_fd, buffer++, 1, 0);
        sz++;
    } while(*(buffer-1) != stop_char);

    /* Remove o caracter de parada da string */
    *(buffer-1) = 0;

    return sz;
}

int main()
{
    int socket_fd, x, y;
    struct sockaddr_in target;
    Map * map;
    Screen * screen;
    Keyboard * keyboard;
    Player * player = new Player(std::make_tuple(0,0));
    char buffer[BUFFER_SIZE] = {0};
    char playercommand, player_id;
    int init_cfg_flag = 0;
    int end_flag = 0;
    int size_x, size_y;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket criado\n");

    target.sin_family = AF_INET;
    target.sin_port = htons(3001);
    inet_aton("127.0.0.1", &(target.sin_addr));

    printf("Tentando conectar\n");

    if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
        printf("Problemas na conexao\n");
        return -1;
    }

    printf("Conectei ao servidor\n");

    /* Lendo configuracoes iniciais */
    while(!init_cfg_flag){
        recv(socket_fd, &buffer[0], 1, 0);

        switch(buffer[0]) {
        case 'X':
            read_until_stop(socket_fd, &buffer[0], ' ');
            size_x = atoi(buffer);
            break;
        case 'Y':
            read_until_stop(socket_fd, &buffer[0], ' ');
            size_y = atoi(buffer);
            break;
        case 'P':
            read_until_stop(socket_fd, &buffer[0], ' ');
            player->set_id(atoi(buffer));
            break;
        case 'L':
            read_until_stop(socket_fd, &buffer[0], ' ');
            player->set_lives(atoi(buffer));
            break;
        case 'S':
            read_until_stop(socket_fd, &buffer[0], ' ');
            player->set_score(atoi(buffer));
            break;
        case 'B':
            read_until_stop(socket_fd, &buffer[0], ' ');
            player->set_bomb_count(atoi(buffer));
            break;

        case 'M':
            /* Server vai mandar dados do mapa, sai do loop de inicializacao */
            init_cfg_flag = 1;
            break;
        }
    }

    printf("Inicializando a tela\n");

    map = new Map(size_y, size_x);
    screen = new Screen(map, player);
    keyboard = new Keyboard();
    keyboard->init();

    while (end_flag == 0) {
	if (!init_cfg_flag) {
	    recv(socket_fd, &buffer[0], 1, 0);
	} else {
	    buffer[0] = 'M';
	    init_cfg_flag = 0;
	}

        switch(buffer[0]) {
        case 'M':
            for (int y = 0; y < size_y; y++) {
                for (int x = 0; x < size_x; x++) {
                    read_until_stop(socket_fd, &buffer[0], ' ');
                    map->array[y][x] = std::bitset<MAX_FLAGS>(std::string(buffer));
                }
            }
            break;

        case 'L':
            read_until_stop(socket_fd, &buffer[0], ' ');
            player->set_lives(atoi(buffer));
            break;
        case 'S':
            read_until_stop(socket_fd, &buffer[0], ' ');
            player->set_score(atoi(buffer));
            break;
        case 'B':
            read_until_stop(socket_fd, &buffer[0], ' ');
            player->set_bomb_count(atoi(buffer));
            break;

	case 'Q':
	case 'q':
	    end_flag = 1;
	    break;

	default:
	    break;
        }

	screen->update();
    
    playercommand = keyboard->getchar();
    send(socket_fd, &playercommand, 1, 0);
    sleep(0.5);
    }

    delete screen;
    delete player;
    delete map;
    close(socket_fd);

    return 0;
}

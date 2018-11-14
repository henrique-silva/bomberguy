#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "controller.hpp"
#include "keyboard.hpp"

#define MAX_PLAYERS 2

using namespace std::chrono;

uint64_t get_now_ms() {
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{
    char c;
    uint64_t t0;
    uint64_t t1;
    uint64_t deltaT;
    uint64_t T;
    int x, y;

    Position pos;

    int server_fd;
    struct sockaddr_in myself, client;
    socklen_t client_size = (socklen_t)sizeof(client);
    char input_buffer[50];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket criado\n");

    /* Enable address reuse */
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
    }

    /* Configure linger */
    linger lin;
    lin.l_onoff = 0;
    lin.l_linger = 0;
    setsockopt(server_fd, SOL_SOCKET, SO_LINGER, (const char *)&lin, sizeof(int));

    /* Config address */
    myself.sin_family = AF_INET;
    myself.sin_port = htons(3001);
    inet_aton("127.0.0.1", &(myself.sin_addr));

    /* Bind server */
    printf("Tentando abrir porta 3001\n");
    if (bind(server_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
        printf("Problemas ao abrir porta\n");
        return 0;
    }
    printf("Abri porta 3001!\n");

    /* Listen */
    listen(server_fd, 2);
    printf("Estou ouvindo na porta 3001!\n");

    fd_set server_set;
    int clients[MAX_PLAYERS] = {0};
    int max_fd = server_fd;

    Controller *control = new Controller(20, 40);

    Player *new_player;
    int connected_players = 0;

    FD_ZERO(&server_set);
    FD_SET(server_fd, &server_set);

    /* Connect all clients */
    while (connected_players < MAX_PLAYERS) {

        int client_fd = accept(server_fd, NULL, NULL);

        printf("New player connected: FD %d!\r\n", client_fd);

        /* Add client fd to our server fd_set */
        FD_SET(client_fd, &server_set);
        clients[connected_players++] = client_fd;
        if (client_fd > max_fd) {
            max_fd = client_fd;
        }

        /* Add player and save its FD */
        new_player = new Player(client_fd, std::make_tuple(1,2*connected_players));
        control->add_player(new_player);
    }

    control->init_game();

    T = get_now_ms();
    t1 = T;

    while (control->get_game_status()) {
        t0 = t1;
        t1 = get_now_ms();
        deltaT = t1-t0;

        control->update((double)deltaT);

        fd_set read_fd = server_set;

        /* Set seelct timeout to 0s, so it won't block */
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 10000;

        int socket_cnt = select(max_fd+1, &read_fd, NULL, NULL, &tv);

        for (int i = 0; i <= max_fd; i++) {
            if ( FD_ISSET(i, &read_fd ) && i != server_fd ) {
                /* Ignore new connections, just proccess messages */
                if (recv(i, &c, 1, MSG_DONTWAIT) > 0) {
                    Player *p = control->find_player_by_fd(i);

                    printf("Received message from player %d: %c\r\n", p->get_id(), c);

                    switch (c) {
                    case 'w':
                    case 'W':
                        printf("Moving UP\r\n");
                        pos = control->move_player(p, UP);
                        break;

                    case 'a':
                    case 'A':
                        printf("Moving LEFT\r\n");
                        pos = control->move_player(p, LEFT);
                        break;

                    case 's':
                    case 'S':
                        printf("Moving DOWN\r\n");
                        pos = control->move_player(p, DOWN);
                        break;

                    case 'd':
                    case 'D':
                        printf("Moving RIGHT\r\n");
                        pos = control->move_player(p, RIGHT);
                        break;

                    case ' ':
                        printf("Drop bomb\r\n");
                        control->drop_bomb(p, 2500);
                        break;

                    case 'q':
                    case 'Q':
                        printf("Quit\r\n");
                        control->remove_player(p);
                        break;

                    default:
                        printf("ERROR on decoding message: \"%c\" int: %d\r\n", c, (uint8_t)c);
                    }
                }
            }
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    delete control;

    close(server_fd);

    return 0;
}

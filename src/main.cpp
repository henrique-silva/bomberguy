#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "controller.hpp"
#include "keyboard.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std::chrono;

uint64_t get_now_ms() {
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{
    int c;
    uint64_t t0;
    uint64_t t1;
    uint64_t deltaT;
    uint64_t T;
    int x, y;

    Position pos;

    int socket_fd, connection_fd;
    struct sockaddr_in myself, client;
    socklen_t client_size = (socklen_t)sizeof(client);
    char input_buffer[50];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket criado\n");

    myself.sin_family = AF_INET;
    myself.sin_port = htons(3001);
    inet_aton("127.0.0.1", &(myself.sin_addr));

    printf("Tentando abrir porta 3001\n");
    if (bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
        printf("Problemas ao abrir porta\n");
        return 0;
    }
    printf("Abri porta 3001!\n");

    listen(socket_fd, 2);
    printf("Estou ouvindo na porta 3001!\n");

    Player *player = new Player(std::make_tuple(1, 1));

    //Keyboard *keyboard = new Keyboard();

    Controller *control = new Controller(player, 10, 20);

    //keyboard->init();

    T = get_now_ms();
    t1 = T;

    connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
    while (control->get_game_status()) {
        t0 = t1;
        t1 = get_now_ms();
        deltaT = t1-t0;

        control->update((double)deltaT);

        if (recv(connection_fd, &c, 1, MSG_DONTWAIT) > 0) {
            switch (c) {
            case 'w':
            case 'W':
            case KEY_UP:
                pos = control->move_player(UP);
                break;

            case 'a':
            case 'A':
            case KEY_LEFT:
                pos = control->move_player(LEFT);
                break;

            case 's':
            case 'S':
            case KEY_DOWN:
                pos = control->move_player(DOWN);
                break;

            case 'd':
            case 'D':
            case KEY_RIGHT:
                pos = control->move_player(RIGHT);
                break;

            case ' ':
                control->drop_bomb(2500);
                break;

            case 'q':
            case 'Q':
                control->set_game_status(false);
                break;
            }
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    close(socket_fd);
    //delete keyboard;
    delete control;
    delete player;

    return 0;
}

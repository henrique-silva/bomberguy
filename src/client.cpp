#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ncurses.h>

int main() {
    int socket_fd;
    struct sockaddr_in target;

    int c;

    initscr();                  /* Start curses mode              */
    keypad(stdscr, TRUE);    /* We get F1, F2 etc..         */
    noecho();                   /* Don't echo() while we do getch */
    curs_set(0);                /* Do not display cursor */

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    printw("Socket criado\n");
    refresh();

    target.sin_family = AF_INET;
    target.sin_port = htons(3001);
    inet_aton("127.0.0.1", &(target.sin_addr));

    printw("Tentando conectar\n");
    refresh();

    if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
        printw("Problemas na conexao\n");
        refresh();
        return -1;
    }

    printw("Conectei ao servidor\n");
    printw("Redirecionando o teclado para servidor! Pressione 'q' para sair\n");
    refresh();

    while (c != 'q') {
        c = getch();
        send(socket_fd, &c, 1, 0);
        sleep(0.5);
    }

    close(socket_fd);
    endwin();
    return 0;
}

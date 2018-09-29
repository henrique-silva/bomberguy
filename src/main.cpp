#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ncurses.h>

#include "controller.hpp"
#include "keyboard.hpp"

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

    Player *player = new Player(std::make_tuple(1, 1));

    Keyboard *keyboard = new Keyboard();

    Controller *control = new Controller(player, 10, 20);

    keyboard->init();

    T = get_now_ms();
    t1 = T;

    while (control->get_game_status()) {
	t0 = t1;
        t1 = get_now_ms();
        deltaT = t1-t0;

	control->update((double)deltaT);
        c = keyboard->getchar();

        switch (c) {
        case KEY_UP:
            pos = control->move_player(UP);
            break;

        case KEY_LEFT:
            pos = control->move_player(LEFT);
            break;

        case KEY_DOWN:
            pos = control->move_player(DOWN);
            break;

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

	std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    delete keyboard;
    delete control;
    delete player;

    return 0;
}

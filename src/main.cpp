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

    Player *p1 = new Player(std::make_tuple(1, 1));
    Player *p2 = new Player(std::make_tuple(1, 3));

    Keyboard *keyboard = new Keyboard();

    Controller *control = new Controller(10, 20);
    control->add_player(p1);
    control->add_player(p2);
    control->init();

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
	case 'w':
	case 'W':
            pos = control->move_player(p1, UP);
            break;

        case KEY_UP:
            pos = control->move_player(p2, UP);
            break;

	case 'a':
	case 'A':
            pos = control->move_player(p1, LEFT);
            break;

        case KEY_LEFT:
            pos = control->move_player(p2, LEFT);
            break;

	case 's':
	case 'S':
            pos = control->move_player(p1, DOWN);
            break;

        case KEY_DOWN:
            pos = control->move_player(p2, DOWN);
            break;

	case 'd':
	case 'D':
	    pos = control->move_player(p1, RIGHT);
            break;

        case KEY_RIGHT:
            pos = control->move_player(p2, RIGHT);
            break;

        case ' ':
            control->drop_bomb(p1, 2500);
            break;

	case 'X':
	case 'x':
            control->drop_bomb(p2, 2500);
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
    //delete p1;
    //delete p2;

    return 0;
}

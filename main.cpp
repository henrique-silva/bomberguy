#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "model.hpp"

#include "screen.hpp"
#include "controller.hpp"
#include "keyboard.hpp"

using namespace std::chrono;

int main ()
{
    int c;
    uint64_t t0;
    uint64_t t1;
    uint64_t deltaT;
    uint64_t T;
    int x, y;

    Level *lvl = new Level(15, 5, 1);

    Player *player = new Player(std::make_tuple(1, 1));

    Screen *screen = new Screen(lvl);

    Keyboard *keyboard = new Keyboard();

    Controller *control = new Controller(screen, lvl, player);

    screen->init();
    screen->update();

    keyboard->init();
    x = y = 1;
    while (1) {
        Position pos;
        c = keyboard->getchar();

        switch (c) {
        case KEY_UP:
            pos = control->move_player(std::make_tuple(x, y-1));
            x = std::get<0>(pos);
            y = std::get<1>(pos);
            break;

        case KEY_LEFT:
            pos = control->move_player(std::make_tuple(x-1, y));
            x = std::get<0>(pos);
            y = std::get<1>(pos);
            break;

        case KEY_DOWN:
            pos = control->move_player(std::make_tuple(x, y+1));
            x = std::get<0>(pos);
            y = std::get<1>(pos);
            break;

        case KEY_RIGHT:
            pos = control->move_player(std::make_tuple(x+1, y));
            x = std::get<0>(pos);
            y = std::get<1>(pos);
            break;

        case ' ':
            control->drop_bomb(std::make_tuple(x, y), 3, 1);
	    sound_player->play(door_sample);
            break;

        case 'q':
        case 'Q':
            keyboard->stop();
            screen->stop();
            return 0;
        }

        if (c) {
            screen->update();
        }
    }
}

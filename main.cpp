#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ncurses.h>

#include "audio.hpp"
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

    /* TODO: Cleanup these pointers when ending the game */
    /* Load all audio samples */
    Audio::Sample *bg_music = new Audio::Sample();
    bg_music->load("assets/level.dat", 0.2);

    Audio::Sample *door_sample = new Audio::Sample();
    door_sample->load("assets/sfx/door_discover.dat");

    Audio::Sample *bomb_drop_sample = new Audio::Sample();
    bomb_drop_sample->load("assets/sfx/bomb_drop.dat");

    Audio::Sample *silence_sample = new Audio::Sample();
    silence_sample->load("assets/silence.dat");

    /* Audio initialization */
    Audio::Player *background_player = new Audio::Player();
    background_player->init();
    background_player->play(silence_sample);

    Audio::Player *sfx_player = new Audio::Player();
    sfx_player->init();
    sfx_player->play(silence_sample);

    /* Start background track */
    background_player->play(bg_music);

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
            if (control->drop_bomb(std::make_tuple(x, y), 3, 1)) {
		sfx_player->play(bomb_drop_sample);
	    }
            break;

        case 'q':
        case 'Q':
            keyboard->stop();
            screen->stop();
            background_player->stop();
            sfx_player->stop();
            return 0;
        }

        if (c) {
            screen->update();
        }
    }
}

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

    Level *lvl = new Level(15, 5, 1);

    Player *player = new Player(std::make_tuple(1, 2));

    Screen *screen = new Screen(lvl);

    Keyboard *keyboard = new Keyboard();

    Controller *control = new Controller(screen, lvl, player);

    screen->init();
    screen->update();

    keyboard->init();

    /* TODO: Cleanup these pointers when ending the game */
    /* Load all audio samples */
    Audio::Sample *bg_music = new Audio::Sample();
    Audio::Sample *door_sample = new Audio::Sample();
    Audio::Sample *bomb_drop_sample = new Audio::Sample();

    bomb_drop_sample->load("assets/sfx/bomb_drop.wav");
    //bg_music->load("assets/wav/level.wav");
    //door_sample->load("assets/sfx/door_discover.wav");


    //Audio::Sample *silence_sample = new Audio::Sample();
    //silence_sample->load("assets/silence.dat");

    /* Audio initialization */
    Audio::Player *background_player = new Audio::Player();
    //background_player->init();
    //background_player->play(silence_sample);

    Audio::Player *sfx_player = new Audio::Player();
    //sfx_player->init();
    //sfx_player->play(silence_sample);

    /* Start background track */
    //background_player->play(bg_music);

    T = get_now_ms();
    t1 = T;
    x = y = 1;
    control->move_player(std::make_tuple(x, y));

    while (1) {
	t0 = t1;
        t1 = get_now_ms();
        deltaT = t1-t0;

	control->update((double)deltaT);
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
            if (control->drop_bomb(std::make_tuple(x, y), 3000)) {
		//sfx_player->play(bomb_drop_sample);
	    }
            break;

        case 'q':
        case 'Q':
            keyboard->stop();
            screen->stop();
            //background_player->stop();
            //sfx_player->stop();
            return 0;
        }

	screen->update();

	std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
}

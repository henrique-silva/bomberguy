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

    T = get_now_ms();
    t1 = T;
    x = y = 1;
    control->move_player(std::make_tuple(x, y));

    while (control->get_game_status()) {
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
	    control->set_game_status(false);
	    break;
        }

	screen->update();

	std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    keyboard->stop();
    screen->stop();
    //background_player->stop();
    //sfx_player->stop();
    return 0;

}

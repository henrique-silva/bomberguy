#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "model.hpp"

#include "screen.hpp"
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


    Screen *screen = new Screen(lvl);

    Keyboard *keyboard = new Keyboard();


    screen->init();
    screen->update();

    keyboard->init();
    x = y = 1;
    while (1) {
        Position pos;
        c = keyboard->getchar();

        switch (c) {
            break;


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


/**********************************************************************************/
/******************* MOVE ALL THIS CODE TO CONTROLLER PART ************************/
/**********************************************************************************/

/* Checks if the move is valid (won't hit anything) */
int valid_move( Level& lvl, int y, int x )
{

}

/**********************************************************************************/
/**********************************************************************************/

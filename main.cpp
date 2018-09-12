#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "model.hpp"

#define SCALE 1
#define SCALE_X SCALE*2
#define SCALE_Y SCALE
#define MOVE_SCALE(y, x) move(y*SCALE_Y, x*SCALE_X)

using namespace std::chrono;

int main ()
{
    char c;
    uint64_t t0;
    uint64_t t1;
    uint64_t deltaT;
    uint64_t T;
    int x, y;

    Level *lvl = new Level(15, 1, 1);

    /**********************************************************************************/
    /************************* MOVE ALL THIS CODE TO VIEW PART ************************/
    /**********************************************************************************/

    initscr();                         /* Start curses mode             */
    raw();                                       /* Line buffering disabled     */
    keypad(stdscr, TRUE);        /* We get F1, F2 etc..         */
    noecho();                            /* Don't echo() while we do getch */
    curs_set(0);           /* Do not display cursor */


    for ( const auto& w : lvl->get_walls() ) {
	y = get<1>(w);
	x = get<0>(w);
        MOVE_SCALE(y, x);   /* Move cursor to position */
	for ( int j = 0; j < SCALE_Y; j++) {
	    for ( int i = 0; i < SCALE_X; i++) {
		addch(' '|A_REVERSE);
	    }
	    move((y*SCALE_Y)+j+1, x*SCALE_X);
	}
    }

    /**********************************************************************************/
    /**********************************************************************************/

    x = 1;
    y = 1;
    MOVE_SCALE(y, x);
    addstr("<>");  /* Prints character, advances a position */
    refresh();      /* Refresh screen */

    /**********************************************************************************/
    /***************************** KEYBOARD THREAD/CLASS ******************************/
    /**********************************************************************************/

    while (1) {
        c = getch();

	MOVE_SCALE(y, x);
	addstr("  ");  /* Prints character, advances a position */
        switch (c) {
        case 'w':
        case 'W':
	    y--;
            break;

	case 'a':
        case 'A':
	    x--;
	    break;

        case 's':
        case 'S':
	    y++;
            break;

        case 'd':
        case 'D':
	    x++;
	    break;

	case 'q':
	case 'Q':
	    endwin(); /* End curses mode */
	    return 0;
        }

	MOVE_SCALE(y, x);
	addstr("<>");  /* Prints character, advances a position */
    }

    /**********************************************************************************/
    /**********************************************************************************/

    return 0;
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

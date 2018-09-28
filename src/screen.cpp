#include "screen.hpp"

Screen::Screen(Map *map)
{
    this->map = map;
    this->win = initscr();      /* Start curses mode             */
    raw();                      /* Line buffering disabled     */
    keypad(this->win, TRUE);    /* We get F1, F2 etc..         */
    noecho();                   /* Don't echo() while we do getch */
    curs_set(0);                /* Do not display cursor */
}

Screen::~Screen()
{
    endwin();
}

WINDOW* Screen::get_window()
{
    return this->win;
}

void Screen::update()
{
    for (int y=0; y <= this->map->get_size_y(); y++) {
        for (int x=0; x <= this->map->get_size_x(); x++) {
	    move(y,x);
	    /* Print symbols from the top-most map (bricks->powerups for example) */
	    if (this->map->has_flag(y, x, FLAG_WALL)) {
                attron(A_REVERSE);
                addch(' ');
                attroff(A_REVERSE);

	    } else if (this->map->has_flag(y, x, FLAG_FLAME)) {
		addch('X');

	    } else if (this->map->has_flag(y, x, FLAG_BRICK)) {
                attron(A_DIM);
                addch('#');
                attroff(A_DIM);

	    } else if (this->map->has_flag(y, x, FLAG_PLAYER)) {
		addch('A');

	    } else if (this->map->has_flag(y, x, FLAG_ENEMY)) {
		addch('@');

	    } else if (this->map->has_flag(y, x, FLAG_BOMB)) {
		addch('o');

	    } else if (this->map->has_flag(y, x, FLAG_DOOR)) {
		addch('D');

	    } else if (this->map->has_flag(y, x, FLAG_PWR_BOMB)) {
		addch('B');

	    } else if (this->map->has_flag(y, x, FLAG_PWR_FLAME)) {
		addch('F');

	    } else if (this->map->has_flag(y, x, FLAG_PWR_LIFE)) {
		addch('L');

	    } else {
		addch(' ');
	    }
	}
    }

    refresh();      /* Refresh screen */
}

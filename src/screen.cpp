#include "screen.hpp"

Screen::Screen(Level *lvl)
{
    this->level = lvl;
    this->map = this->map_old = lvl->get_map();
}

void Screen::init()
{
    this->win = initscr();      /* Start curses mode             */
    raw();                      /* Line buffering disabled     */
    keypad(this->win, TRUE);    /* We get F1, F2 etc..         */
    noecho();                   /* Don't echo() while we do getch */
    curs_set(0);                /* Do not display cursor */
}

void Screen::stop()
{
    endwin();
}

Screen::~Screen()
{
    this->stop();
}

WINDOW* Screen::get_window()
{
    return this->win;
}

void Screen::update()
{
    for (int i=0; i< this->map->size(); i++) {
	for (int j=0; j < this->map->size(); j++) {
	    switch ((*this->map)[i][j]) {
	    case SYMBOL_WALL:
		move(j,i);
		attron(A_REVERSE);
		addch(' ');
		attroff(A_REVERSE);
		break;

	    case SYMBOL_DOOR_HIDDEN:
		/* Door is hidden behind a brick, so display as the same character */
	    case SYMBOL_BRICK:
		move(j,i);
		attron(A_DIM);
		addch('#');
		attroff(A_DIM);
		break;

	    case SYMBOL_SPACE:
		mvaddch(j, i, ' ');
		break;

	    case SYMBOL_PLAYER:
		mvaddch(j, i, 'A');
		break;

	    case SYMBOL_DOOR_FOUND:
		mvaddch(j, i, 'D');
		break;

	    case SYMBOL_BOMB:
		mvaddch(j, i, 'o');
		break;

	    case SYMBOL_EXPLOSION:
		mvaddch(j, i, 'X');
		break;

	    case SYMBOL_ENEMY:
		mvaddch(j, i, 'E');
		break;

	    }
	}
    }

    refresh();      /* Refresh screen */
}

#include "screen.hpp"

Screen::Screen(Map *map, Player *player)
{
    this->map = map;
    this->player = player;

    initscr();      /* Start curses mode             */

    this->update_limits();

    int map_y = this->map->get_size_y();
    int map_x = this->map->get_size_x();

    this->map_win = newwin(map_y+1, map_x+1, (this->max_y - map_y)/2, (this->max_x - map_x)/2);
    this->info_win = newwin(4, map_x, ((this->max_y - map_y)/2) - 4, (this->max_x - map_x)/2);

    raw();                      /* Line buffering disabled     */
    keypad(this->map_win, TRUE);    /* We get F1, F2 etc..         */
    keypad(stdscr, TRUE);    /* We get F1, F2 etc..         */
    noecho();                   /* Don't echo() while we do getch */
    curs_set(0);                /* Do not display cursor */

}

Screen::~Screen()
{
    delwin(this->map_win);
    endwin();
}

WINDOW* Screen::get_map_window()
{
    return this->map_win;
}

void Screen::update_limits()
{
    getmaxyx(stdscr, this->max_y, this->max_x);
}

void Screen::loading_page()
{
    int y,x;

    update_limits();
    y = this->max_y/2 - 5;
    x = this->max_x/2 - 30;
    //move(y, x);
    for (int i = 0; i < sizeof(title)/sizeof(title[0]); i++, y++) {
	mvprintw(y, x, title[i].c_str());
    }

    mvprintw(y+1, x, "Loading...");

    wrefresh(stdscr);
}

void Screen::start_map_screen()
{
    clear();
    refresh();

    this->map_offset_y = (this->max_y - this->map->get_size_y())/2;
    this->map_offset_x = (this->max_x - this->map->get_size_y())/2;
    this->update();
}

void Screen::update()
{
    this->update_limits();

    for (int y=0; y <= this->map->get_size_y(); y++) {
        for (int x=0; x <= this->map->get_size_x(); x++) {
	    wmove(this->map_win, y, x);
	    /* Print symbols from the top-most map (bricks->powerups for example) */
	    if (this->map->has_flag(y, x, FLAG_WALL)) {
                wattron(this->map_win, A_REVERSE);
                waddch(this->map_win,' ');
                wattroff(this->map_win, A_REVERSE);

	    } else if (this->map->has_flag(y, x, FLAG_FLAME)) {
		waddch(this->map_win,'X');

	    } else if (this->map->has_flag(y, x, FLAG_BRICK)) {
                wattron(this->map_win, A_DIM);
                waddch(this->map_win,'#');
                wattroff(this->map_win, A_DIM);

	    } else if (this->map->has_flag(y, x, FLAG_PLAYER)) {
		waddch(this->map_win,'A');

	    } else if (this->map->has_flag(y, x, FLAG_ENEMY)) {
		waddch(this->map_win,'@');

	    } else if (this->map->has_flag(y, x, FLAG_BOMB)) {
		waddch(this->map_win,'o');

	    } else if (this->map->has_flag(y, x, FLAG_DOOR)) {
		waddch(this->map_win,'D');

	    } else if (this->map->has_flag(y, x, FLAG_PWR_BOMB)) {
		waddch(this->map_win,'B');

	    } else if (this->map->has_flag(y, x, FLAG_PWR_FLAME)) {
		waddch(this->map_win,'F');

	    } else if (this->map->has_flag(y, x, FLAG_PWR_LIFE)) {
		waddch(this->map_win,'L');

	    } else {
		waddch(this->map_win,' ');
	    }
	}
    }

    wrefresh(this->map_win);      /* Refresh screen */

    mvwprintw(this->info_win, 1, 1, "Lives: %d", this->player->get_lives());
    mvwprintw(this->info_win, 2, 1, "Bombs: %d", this->player->get_bomb_count());
    mvwprintw(this->info_win, 3, 1, "Score: %d", this->player->get_score());

    wrefresh(this->info_win);      /* Refresh screen */
}

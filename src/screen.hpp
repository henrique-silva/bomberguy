#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <ncurses.h>
#include "map.hpp"
#include "player.hpp"

class Screen {
  private:
    WINDOW *info_win;
    WINDOW *map_win;

    int max_y;
    int max_x;

    Map *map;
    int map_offset_y;
    int map_offset_x;

    Player* player;

    std::string title[5] = { "   ___                __                             ",
			     "  / _ ) ___   __ _   / /  ___  ____ ___ _ __ __ __ __",
			     " / _  |/ _ \\ /  ' \\ / _ \\/ -_)/ __// _ `// // // // /",
			     "/____/ \\___//_/_/_//_.__/\\__//_/   \\_, / \\_,_/ \\_, / ",
			     "                                  /___/       /___/  "};

public:
    Screen(Map *map, Player *player);
    ~Screen();
    WINDOW *get_map_window();
    void update();
    void update_limits();
    void loading_page();
    void start_map_screen();
};

#endif

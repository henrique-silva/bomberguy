#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <ncurses.h>
#include "map.hpp"

class Screen {
  private:
    WINDOW *win;
    Map *map;

  public:
    Screen(Map *map);
    ~Screen();
    WINDOW *get_window();
    void update();
};

#endif

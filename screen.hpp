#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <ncurses.h>
#include "model.hpp"

class Screen {
  private:
    WINDOW *win;
    Level *level;
    Map *map;
    Map *map_old;

  public:
    Screen(Level *level);
    ~Screen();
    WINDOW *get_window();
    void stop();
    void init();
    void update();
};

#endif

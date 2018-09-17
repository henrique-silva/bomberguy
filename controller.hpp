#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "screen.hpp"
#include "model.hpp"
#include "graphics.hpp"

class Controller {
private:
    Screen *screen;
    Level *level;
    Player *player;
    std::vector<Bomb *> bomb_list;
    //vector<Enemy*> enemy_list;

public:
    Controller(Screen *scr, Level *lvl, Player *player);
    ~Controller();
    void drop_bomb(Position pos, int remaining_time, int range);
    int is_colision(Position pos);
    Position move_player(Position new_pos);
};


#endif

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "screen.hpp"
#include "model.hpp"

#define EXPLOSION_WEAROFF_TIME 500

class Controller {
private:
    Screen *screen;
    Level *level;
    Player *player;
    std::vector<Bomb *> bomb_list;
    //vector<Enemy*> enemy_list;

    /* Left, Up, Right, Down */
    std::vector<int> dir_x {-1, 0, 1, 0};
    std::vector<int> dir_y {0, 1, 0, -1};


public:
    Controller(Screen *scr, Level *lvl, Player *player);
    ~Controller();
    int is_colision(Position pos);
    Position move_player(Position new_pos);
    void update(int deltaT);

    int drop_bomb(Position pos, int remaining_time, int range);
    void remove_bomb(Bomb *bomb);
    void explode_bomb(Bomb *bomb);
};


#endif

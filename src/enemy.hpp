#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "util.hpp"

class Enemy {
private:
    int score;
    Position pos;
    Velocity vel;

public:
    Enemy(Position pos, Velocity vel, int score);

    int get_score();
    Position get_pos();
    void set_pos(Position new_pos);
    Velocity get_velocity();
    void set_velocity(Velocity new_vel);
};

#endif

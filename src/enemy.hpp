#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <tuple>

//typedef std::tuple<double, double> Velocity;
using Velocity = std::tuple<double, double>;
using Position_d = std::tuple<double, double>;

class Enemy {
private:
    int score;
    Position_d pos;
    Velocity vel;

public:
    Enemy(Position_d pos, Velocity vel, int score);

    int get_score();
    Position_d get_pos();
    void set_pos(Position_d new_pos);
    Velocity get_velocity();
    void set_velocity(Velocity new_vel);
};

#endif

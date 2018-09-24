#include "enemy.hpp"

/********************************************************************/
/*********************** Enemy Class methods ************************/
/********************************************************************/
Enemy::Enemy(Position pos, Velocity vel, int score)
{
    this->pos = pos;
    this->vel = vel;
    this->score = score;
}

int Enemy::get_score()
{
    return this->score;
}

Position Enemy::get_pos()
{
    return this->pos;
}

void Enemy::set_pos(Position new_pos)
{
    this->pos = new_pos;
}

Velocity Enemy::get_velocity()
{
    return this->vel;
}

void Enemy::set_velocity(Velocity new_vel)
{
    this->vel = new_vel;
}

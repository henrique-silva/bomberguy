#include "bomb.hpp"

/*********************** Bomb Class methods ************************/
Bomb::Bomb(Player *owner, Position pos, int remaining_time, int range)
{
    this->pos = pos;
    this->remaining_time = remaining_time;
    this->range = range;
    this->status = BOMB_ARMED;
    this->owner = owner;
}

Player* Bomb::get_owner()
{
    return this->owner;
}

int Bomb::get_remaining_time(void)
{
    return this->remaining_time;
}

int Bomb::get_range(void)
{
    return this->range;
}

void Bomb::set_remaining_time(int new_time)
{
    this->remaining_time = new_time;
}

int Bomb::get_status(void)
{
    return this->status;
}

void Bomb::set_status(int sts)
{
    this->status = sts;
}

Position Bomb::get_pos()
{
    return this->pos;
}

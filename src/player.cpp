#include "player.hpp"

/*********************** Player Class methods ************************/
Player::Player(Position init_pos)
{
    this->pos = init_pos;
    this->bomb_count = 1;
    this->bomb_range = 2;
    this->score = 0;
}

int Player::get_score()
{
    return this->score;
}

void Player::set_score(int new_score)
{
    this->score = new_score;
}

int Player::get_bomb_count()
{
    return this->bomb_count;
}

void Player::set_bomb_count(int new_count)
{
    /* Avoid negative bomb count */
    if (new_count >= 0) {
	this->bomb_count = new_count;
    }
}

void Player::add_bomb()
{
    this->bomb_count++;
}

void Player::remove_bomb()
{
    /* Avoid negative bomb count */
    if (this->bomb_count > 0) {
	this->bomb_count--;
    }
}

int Player::get_bomb_range()
{
    return this->bomb_range;
}

void Player::set_bomb_range(int new_range)
{
    /* Avoid ultra-small bomb range */
    if (new_range >= 1) {
	this->bomb_range = new_range;
    }
}

Position Player::get_pos()
{
    return this->pos;
}

void Player::set_pos(Position new_pos)
{
    this->pos = new_pos;
}

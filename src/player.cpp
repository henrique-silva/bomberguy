#include "player.hpp"

/*********************** Player Class methods ************************/
Player::Player(int socket_fd)
{
    this->socket_fd = socket_fd;
    this->bomb_count = 1;
    this->bomb_range = 2;
    this->score = 0;
    this->lives = 1;
}

int Player::get_socket_fd()
{
    return this->socket_fd;
}

int Player::get_id()
{
    return this->id;
}

void Player::set_id(int id)
{
    this->id = id;
}

int Player::get_score()
{
    return this->score;
}

void Player::set_score(int new_score)
{
    this->score = new_score;
}

int Player::get_lives()
{
    return this->lives;
}

void Player::set_lives(int new_lives)
{
    this->lives = new_lives;
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

int Player::get_max_bombs()
{
    return this->max_bombs;
}

void Player::set_max_bombs(int new_max)
{
    /* At least one bomb */
    if (new_max > 0) {
        this->max_bombs = new_max;
        /* Include the new bomb in the inventory */
        this->add_bomb();
    }
}

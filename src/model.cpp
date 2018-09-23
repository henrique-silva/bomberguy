#include "model.hpp"

using namespace std;

/*********************** Level Class methods ************************/
Level::Level(int size, int door_x, int door_y)
{
    int x, y;

    /* Use only odd numbers for map size, if even, round it down */
    this->size = (size % 2) ? size : (size-1);

    /* Initialize the map matrix with 0 */
    this->map.resize(size, std::vector<char>(size,' '));

    /* Draw const parts of the map */
    this->build_walls();
    this->min_x = 0;
    this->min_y = 0;
    this->max_x = size;
    this->max_y = size;
    this->set_symbol(make_tuple(door_x, door_y), SYMBOL_DOOR_HIDDEN);
}

void Level::build_walls( void )
{
    int x, y;

    /* Creates a map of the following type:
     *            *******
     *            *     *
     *            * * * *
     *            *     *
     *            * * * *
     *            *     *
     *            *******
     */
    for (x = 0; x < this->size; x++) {
        for (y = 0; y < this->size; y++) {
            if (x == 0 || x == this->size-1) {
                /* First and last lines are all walls*/
                this->map[x][y] = SYMBOL_WALL;
            } else if ((x % 2) == 0) {
                if ((y % 2) == 0) {
		    this->map[x][y] = SYMBOL_WALL;
                }
            } else {
                if (y == 0 || y == this->size-1) {
		    this->map[x][y] = SYMBOL_WALL;
                }
            }
        }
    }
}

Map *Level::get_map()
{
    return &this->map;
}

void Level::set_symbol(Position pos, int symbol)
{
    this->map[(int) floor(get<0>(pos))][(int) floor(get<1>(pos))] = symbol;
}

int Level::get_symbol(Position pos)
{
    int x = (int) floor(get<0>(pos));
    int y = (int) floor(get<1>(pos));
    if ((x >= 0) && (x < this->size) &&
	(y >= 0) && (y < this->size) ) {
	return this->map[x][y];
    } else {
	return 0;
    }
}

Position Level::get_door(void)
{
    int index_y, index_x;

    for (Map::iterator it = this->map.begin(); it != this->map.end(); it++) {
        auto elem = find(it->begin(), it->end(), SYMBOL_DOOR_FOUND);
        if (elem != it->end()) {
            /* Found Door in map matrix */
            auto index_x = distance(this->map.begin(), it);
            auto index_y = distance(it->begin(), elem);
        }
    }

    return make_tuple(index_x, index_y);
}

/*********************** Bomb Class methods ************************/
Bomb::Bomb(Position pos, int remaining_time, int range)
{
    this->pos = pos;
    this->remaining_time = remaining_time;
    this->range = range;
    this->status = BOMB_ARMED;
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

Position Player::get_pos()
{
    return this->pos;
}

void Player::set_pos(Position new_pos)
{
    this->pos = new_pos;
}

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

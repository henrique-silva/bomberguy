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
    this->set_door(make_tuple(door_x, door_y));
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

void Level::set_door(Position pos)
{
    this->map[get<0>(pos)][get<1>(pos)] = SYMBOL_DOOR;
}

void Level::set_bomb(Position pos)
{
    this->map[get<0>(pos)][get<1>(pos)] = SYMBOL_BOMB;
}

void Level::set_space(Position pos)
{
    this->map[get<0>(pos)][get<1>(pos)] = SYMBOL_SPACE;
}

void Level::set_player(Position pos)
{
    this->map[get<0>(pos)][get<1>(pos)] = SYMBOL_PLAYER;
}

int Level::get_type(Position pos)
{
    return this->map[get<0>(pos)][get<1>(pos)];
}

Position Level::get_door(void)
{
    int index_y, index_x;

    for (Map::iterator it = this->map.begin(); it != this->map.end(); it++) {
        auto elem = find(it->begin(), it->end(), 'D');
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
}

int Bomb::get_remaining_time( void )
{
    return this->remaining_time;
}

int Bomb::get_range( void )
{
    return this->range;
}

void Bomb::update_remaining_time(int new_time)
{
    this->remaining_time = new_time;
}

Position Bomb::get_pos()
{
    return this->pos;
}

/*********************** Player Class methods ************************/
Player::Player(Position init_pos)
{
    this->pos = init_pos;
    this->bomb_count = 5;
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
    this->bomb_count = new_count;
}

Position Player::get_pos()
{
    return this->pos;
}

void Player::set_pos(Position new_pos)
{
    this->pos = new_pos;
}

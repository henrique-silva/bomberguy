#include "map.hpp"

/*********************** Map Class methods ************************/
Map::Map(int size_y, int size_x)
{
    /* Seed the random functions */
    srand(time(NULL));

    this->door_found = 0;

    /* size_x = column count */
    /* size_y = line count */
    if ((size_x % 2) == 0) {
        size_x++;
    }
    if ((size_y % 2) == 0) {
        size_y++;
    }
    this->size_x = size_x;
    this->size_y = size_y;

    /* Initialize the map matrix with 0 */
    this->array.resize(size_y, std::vector<std::bitset<MAX_FLAGS> >(size_x, 0));

    /* Draw const parts of the map */
    this->draw_walls();

    /* Add powerups */
    this->add_powerups();
}

void Map::draw_walls( void )
{
    int x, y, i;

    /* Creates a map of the following type:
     *            *******
     *            *     *
     *            * * * *
     *            *     *
     *            * * * *
     *            *     *
     *            *******
     */
    for (y = 0; y < this->size_y; y++) {
        for (x = 0; x < this->size_x; x++) {
            if (x == 0 || x == this->size_x-1) {
                /* First and last lines are all walls*/
                this->array[y][x].set(FLAG_WALL);
            } else if ((x % 2) == 0) {
                if ((y % 2) == 0) {
                    this->array[y][x].set(FLAG_WALL);
                }
            } else {
                if (y == 0 || y == this->size_y-1) {
                    this->array[y][x].set(FLAG_WALL);
                }
            }
        }
    }

    /* Hide door */
    //this->set_flag_random(1, FLAG_DOOR);
}

void Map::set_flag_random(int mod, int flag)
{
    int x,y;

    /* Find a random space */
    for (int i = 0; i < mod; i++) {
        x = rand() % this->size_x;
        y = rand() % this->size_y;
	if ((x > 2 && y > 2 && x < this->size_x-2 && y < this->size_y-2) && is_empty(y, x)) {
	    /* Hide the power-up under a brick */
            this->array[y][x].set(flag);
            this->array[y][x].set(FLAG_BRICK);
        } else {
            /* try again */
            i--;
        }
    }
}

Position Map::set_random_player(int id)
{
    int y, x;
    int i = 0;

    for (int i = 0; i < 1; i++) {
	if ((id % 2) == 0) {
	    y = 1;
	    x = rand() % this->get_size_x();
	} else {
	    x = 1;
	    y = rand() % this->get_size_y();
	}

	if (is_empty(y, x)) {
	    this->array[y][x].set(FLAG_PLAYER_BASE+id);
	} else {
	    i--;
	}
    }
    return std::make_tuple(y, x);
}

void Map::add_powerups()
{
    this->set_flag_random(std::max(this->size_x,this->size_y)*0.3, FLAG_PWR_BOMB);
    this->set_flag_random(std::max(this->size_x,this->size_y)*0.3, FLAG_PWR_FLAME);
    this->set_flag_random(std::max(this->size_x,this->size_y)*0.1, FLAG_PWR_LIFE);
}

void Map::add_bricks()
{
    /* 25 percent of all blocks will become random bricks */
    this->set_flag_random((this->size_x*this->size_y*0.25), FLAG_BRICK);
}

bool Map::is_valid_pos(int y, int x)
{
    return ((x >= 0) && (x < this->size_x) &&
            (y >= 0) && (y < this->size_y) );
}

void Map::clear_flag(int y, int x, int flag)
{
    if (is_valid_pos(y, x)) {
        this->array[y][x].reset(flag);
    }
}

void Map::set_flag(int y, int x, int flag)
{
    if (is_valid_pos(y, x)) {
        this->array[y][x].set(flag);
    }
}

Flag Map::get_flag(int y, int x)
{
    if (is_valid_pos(y, x)) {
        return (Flag) this->array[y][x].to_ulong();
    } else {
        return FLAG_INVALID_BLOCK;
    }
}

bool Map::has_flag(int y, int x, int flag)
{
    if (is_valid_pos(y, x)) {
        return this->array[y][x].test(flag);
    } else {
        return false;
    }
}

bool Map::is_empty(int y, int x)
{
    if (is_valid_pos(y, x)) {
        return this->array[y][x].none();
    } else {
        return false;
    }
}


bool Map::is_walkable(int y, int x)
{
    if (is_valid_pos(y, x)) {
        return !(this->array[y][x].test(FLAG_WALL) ||
                 this->array[y][x].test(FLAG_BRICK) ||
                 this->array[y][x].test(FLAG_BOMB));
    } else {
        return false;
    }
}


std::bitset<MAX_FLAGS> Map::get_flags(int y, int x)
{
    std::bitset<MAX_FLAGS> flags;

    if (is_valid_pos(y, x)) {
        flags = this->array[y][x];
    } else {
        /* out-of-range block, mark as invalid */
        flags.set(FLAG_INVALID_BLOCK);
    }

    return flags;
}

int Map::get_size_x(void)
{
    return this->size_x;
}

int Map::get_size_y(void)
{
    return this->size_y;
}

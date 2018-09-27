#include "model.hpp"

//#define RANDOM_WALLS
#define RANDOM_BRICKS

/*********************** Level Class methods ************************/
Level::Level(int size, int door_x, int door_y)
{
    int x, y;

    /* Seed the random functions */
    srand(time(NULL));

    /* Use only odd numbers for map size, if even, round it down */
    this->size = (size % 2) ? size : (size-1);

    /* Initialize the map matrix with 0 */
    this->map.resize(size, std::vector<char>(size,' '));

    /* Draw const parts of the map */
    this->create_map();
    this->min_x = 0;
    this->min_y = 0;
    this->max_x = size;
    this->max_y = size;
    this->set_symbol(std::make_tuple(door_x, door_y), SYMBOL_DOOR_HIDDEN);
}

void Level::create_map( void )
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

#ifdef RANDOM_WALLS
    /* 10 percent of all blocks will become random walls */
    for (i = 0; i < (this->size*this->size*0.1); i++) {
	x = rand() % this->size;
	y = rand() % this->size;
	this->map[x][y] = SYMBOL_WALL;
    }
#endif

#ifdef RANDOM_BRICKS
    /* 10 percent of all blocks will become random walls */
    for (i = 0; i < (this->size*this->size*0.15); i++) {
	x = (rand() % (this->size-2) + 1);
	y = (rand() % (this->size-2) + 1);
	if (this->map[x][y] == SYMBOL_SPACE) {
	    this->map[x][y] = SYMBOL_BRICK;
	}
    }
#endif
}

Map *Level::get_map()
{
    return &this->map;
}

void Level::set_symbol(Position pos, int symbol)
{
    this->map[(int) floor(std::get<0>(pos))][(int) floor(std::get<1>(pos))] = symbol;
}

int Level::get_symbol(Position pos)
{
    int x = (int) floor(std::get<0>(pos));
    int y = (int) floor(std::get<1>(pos));
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

    return std::make_tuple(index_x, index_y);
}

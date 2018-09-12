#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

#include "model.hpp"

using namespace std;

Level::Level(int size, int door_x, int door_y)
{
    int x, y;

    this->set_door_pos(door_x, door_y);

    /* Creates a map of the following type:
     *            *******
     *            *     *
     *            * * * *
     *            *     *
     *            * * * *
     *            *     *
     *            *******
     */
    for (x = 0; x < size; x++) {
	for (y = 0; y < size; y++) {
	    if (x == 0 || x == size-1) {
		/* First and last lines are all walls*/
		this->add_wall(x, y);
	    } else if ((x % 2) == 0) {
		if ((y % 2) == 0) {
		    this->add_wall(x, y);
		}
	    } else {
		if (y == 0 || y == size-1) {
		    this->add_wall(x, y);
		}
	    }
	}
    }
}

void Level::add_wall(int x, int y)
{
    this->walls.push_back(make_tuple(x, y));
}

void Level::set_door_pos(int x, int y)
{
    this->door_pos = make_tuple(x, y);
}

Position Level::get_door_pos()
{
    return this->door_pos;
}

vector<Position> Level::get_walls()
{
    return this->walls;
}

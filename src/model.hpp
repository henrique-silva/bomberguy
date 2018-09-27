#ifndef MODEL_HPP
#define MODEL_HPP

#include "bomb.hpp"
#include "player.hpp"
#include "enemy.hpp"

#define SYMBOL_PLAYER		'P'
#define SYMBOL_WALL		'W'
#define SYMBOL_BRICK		'K'
#define SYMBOL_BOMB		'B'
#define SYMBOL_ENEMY		'E'
#define SYMBOL_SPACE		' '
#define SYMBOL_DOOR_HIDDEN      'd'
#define SYMBOL_DOOR_FOUND       'D'
#define SYMBOL_EXPLOSION	'X'

class Level {
private:
    int size;
    Position door_pos;
    std::vector<Position> walls;
    Map map;
    void create_map();

public:
    int min_x;
    int max_x;
    int min_y;
    int max_y;

    Level(int size, int door_x, int door_y);
    Map *get_map();
    Position get_door();
    void set_symbol(Position pos, int symbol);
    int get_symbol(Position pos);
};

#endif

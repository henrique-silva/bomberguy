#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <algorithm>
#include <tuple>

#define SYMBOL_PLAYER		'P'
#define SYMBOL_WALL		'W'
#define SYMBOL_BRICK		'K'
#define SYMBOL_BOMB		'B'
#define SYMBOL_ENEMY		'E'
#define SYMBOL_SPACE		' '
#define SYMBOL_DOOR		'D'
#define SYMBOL_EXPLOSION	'X'

typedef std::tuple<int, int> Position;
typedef std::vector<std::vector<char> > Map;

enum {
    BOMB_ARMED,
    BOMB_EXPLODED,
    BOMB_REMOVE
};

class Level {
private:
    int size;
    Position door_pos;
    std::vector<Position> walls;
    Map map;
    void build_walls();

public:
    Level(int size, int door_x, int door_y);
    Map *get_map();
    Position get_door();
    void set_symbol(Position pos, int symbol);
    int get_symbol(Position pos);
};

class Bomb {
private:
    Position pos;
    int remaining_time;
    int range;
    int status;

public:
    Bomb(Position pos, int remaining_time, int range);
    Position get_pos();
    void set_remaining_time(int new_time);
    int get_remaining_time();
    int get_range();
    void set_status(int sts);
    int get_status(void);
};

class Player {
private:
    int score;
    Position pos;
    int bomb_count;

public:
    Player(Position init_pos);
    Position get_pos();
    void set_pos(Position new_pos);
    int get_score();
    void set_score(int new_score);
    int get_bomb_count();
    void set_bomb_count(int new_count);
    void add_bomb();
    void remove_bomb();
};

#endif

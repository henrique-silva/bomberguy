#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include <bitset>

#define RANDOM_BRICKS
#define MAX_POWERUPS_SAME_TYPE 3

typedef enum {
    //FLAG_SPACE, /* The block will be empty (or contain 'space' if none of the flags are set) */
    FLAG_WALL,
    FLAG_BRICK,
    FLAG_DOOR,

    FLAG_PLAYER,
    FLAG_ENEMY,

    FLAG_BOMB,
    FLAG_FLAME,

    /* Power ups */
    FLAG_PWR_BOMB,
    FLAG_PWR_FLAME,
    FLAG_PWR_LIFE,

    FLAG_INVALID_BLOCK,

    MAX_FLAGS
} Flag;

class Map {
private:
    int size_x;
    int size_y;
    std::vector<std::vector<std::bitset<MAX_FLAGS> > > array;

    void draw_walls();
    void add_powerups();
    void set_flag_random(int mod, Flag flag);

public:
    Map(int size_y, int size_x);

    int door_found;
    int get_size_x();
    int get_size_y();
    bool is_valid_pos(int y, int x);
    void clear_flag(int y, int x, Flag flag);
    void set_flag(int y, int x, Flag flag);
    bool has_flag(int y, int x, Flag flag);
    bool is_empty(int y, int x);
    bool is_walkable(int y, int x);
    std::bitset<MAX_FLAGS> get_flags(int y, int x);
};

#endif

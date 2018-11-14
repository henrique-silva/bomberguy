#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include <bitset>
#include <tuple>

#define RANDOM_BRICKS
#define MAX_POWERUPS_SAME_TYPE 3

#define MAX_FLAGS 32

typedef std::tuple<int, int> Position;

typedef enum {
    //FLAG_SPACE, /* The block will be empty (or contain 'space' if none of the flags are set) */
    FLAG_WALL,
    FLAG_BRICK,
    FLAG_DOOR,

    FLAG_ENEMY,

    FLAG_BOMB,
    FLAG_FLAME,

    /* Power ups */
    FLAG_PWR_BOMB,
    FLAG_PWR_FLAME,
    FLAG_PWR_LIFE,

    FLAG_INVALID_BLOCK,

    /* All players will have flags from here to max */
    FLAG_PLAYER_BASE

} Flag;

class Map {
private:
    int size_x;
    int size_y;

    void draw_walls();
    void add_powerups();
    void set_flag_random(int mod, int flag);

public:
    int door_found;
    std::vector<std::vector<std::bitset<MAX_FLAGS> > > array;
    Map(int size_y, int size_x);

    void add_bricks();
    Position set_random_player(int id);

    int get_size_x();
    int get_size_y();
    bool is_valid_pos(int y, int x);
    void clear_flag(int y, int x, int flag);
    void set_flag(int y, int x, int flag);
    Flag get_flag(int y, int x);
    bool has_flag(int y, int x, int flag);
    bool is_empty(int y, int x);
    bool is_walkable(int y, int x);
    std::bitset<MAX_FLAGS> get_flags(int y, int x);
};

#endif

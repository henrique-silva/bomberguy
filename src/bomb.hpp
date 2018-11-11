#ifndef BOMB_HPP
#define BOMB_HPP

#include <tuple>
#include <algorithm>
#include "player.hpp"

typedef std::tuple<int, int> Position;

enum {
    BOMB_ARMED,
    BOMB_EXPLODED,
    BOMB_REMOVE
};

class Bomb {
private:
    Player *owner;
    Position pos;
    int remaining_time;
    int range;
    int status;

public:
    Bomb(Player *player, Position pos, int remaining_time, int range);
    Player* get_owner();
    Position get_pos();
    void set_remaining_time(int new_time);
    int get_remaining_time();
    int get_range();
    void set_status(int sts);
    int get_status(void);
};

#endif

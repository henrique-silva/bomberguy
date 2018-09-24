#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "util.hpp"

class Player {
private:
    int score;
    Position pos;
    int bomb_count;
    int bomb_range;

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
    int get_bomb_range();
    void set_bomb_range(int new_range);
};

#endif

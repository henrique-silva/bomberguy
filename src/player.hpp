#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <tuple>
#include "map.hpp"

typedef std::tuple<int, int> Position;

class Player {
private:
    int score;
    Position pos;
    int bomb_count;
    int bomb_range;
    int max_bombs;
    int lives;
    int id;

public:
    Player(Position init_pos);

    int get_id();
    void set_id(int id);

    //int get_flag();

    Position get_pos();
    void set_pos(Position new_pos);

    int get_score();
    void set_score(int new_score);

    int get_lives();
    void set_lives(int new_count);

    int get_bomb_count();
    void set_bomb_count(int new_count);
    int get_max_bombs();
    void set_max_bombs(int new_max);
    void add_bomb();
    void remove_bomb();
    int get_bomb_range();
    void set_bomb_range(int new_range);

};

#endif

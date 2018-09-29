#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vector>

#include "screen.hpp"
#include "playback.hpp"
#include "player.hpp"
#include "bomb.hpp"
#include "enemy.hpp"
#include "map.hpp"

#define EXPLOSION_WEAROFF_TIME 500

typedef enum {
    LEFT,
    UP,
    RIGHT,
    DOWN
} Direction;

class Controller {
private:
    bool game_status;
    Screen *screen;
    Player *player;
    Map *map;
    std::vector<Bomb *> bomb_list;
    std::vector<Enemy *> enemy_list;

    Audio::Player bg_audio;
    Audio::Player sfx_audio;

    /* Left, Up, Right, Down */
    const std::vector<int> dir_x {-1, 0, 1, 0};
    const std::vector<int> dir_y {0, -1, 0, 1};


public:
    Controller(Player *player, int size_y, int size_x, int enemy_count=4);

    ~Controller();

    void update(double deltaT);
    void check_colisions(void);

    void set_game_status(bool sts);
    bool get_game_status(void);

    Position move_player(Direction dir);
    void kill_player(int y, int x);

    void spawn_enemy(int pos_y, int pos_x, double vel_y, double vel_x, int score);
    /* Random position */
    void spawn_enemy(int score);
    Position_d move_enemy(Enemy *enemy, Position_d new_pos);
    void kill_enemy(Position pos);

    int drop_bomb(int remaining_time);
    void remove_bomb(Bomb *bomb);
    void explode_bomb(Bomb *bomb);
    Bomb *find_bomb(Position pos);

};


#endif

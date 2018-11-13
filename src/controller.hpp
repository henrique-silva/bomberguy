#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <vector>

#include "screen.hpp"
#include "playback.hpp"
#include "player.hpp"
#include "bomb.hpp"
#include "enemy.hpp"
#include "map.hpp"
#include "remote_view.hpp"

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
    Map *map;

    std::vector<Player *>player_list;
    int player_cnt;
    std::vector<Spectator *>spec_list;

    std::vector<Bomb *> bomb_list;
    std::vector<Enemy *> enemy_list;

    Audio::Player bg_audio;
    Audio::Player sfx_audio;

    /* Left, Up, Right, Down */
    const std::vector<int> dir_x {-1, 0, 1, 0};
    const std::vector<int> dir_y {0, -1, 0, 1};

public:
    Controller(int size_y, int size_x);
    ~Controller();

    void init(int enemy_count=4);

    void update(double deltaT);
    void check_colisions(void);

    void set_game_status(bool sts);
    bool get_game_status(void);

    void add_player(Player *player);
    Position move_player(Player *player, Direction dir);
    void remove_player(Player *player);
    void kill_player(Player *player, int y, int x);

    void spawn_enemy(int pos_y, int pos_x, double vel_y, double vel_x, int score);
    /* Random position */
    void spawn_enemy(int score);
    Position_d move_enemy(Enemy *enemy, Position_d new_pos);
    void kill_enemy(Position pos);

    int drop_bomb(Player *player, int remaining_time);
    void remove_bomb(Bomb *bomb);
    void explode_bomb(Bomb *bomb);
    Bomb *find_bomb(Position pos);

};


#endif

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "screen.hpp"
#include "model.hpp"
#include "playback.hpp"

#define EXPLOSION_WEAROFF_TIME 500

class Controller {
private:
    bool game_status;
    Screen *screen;
    Level *level;
    Player *player;

    Audio::Player bg_audio;
    Audio::Player sfx_audio;

    std::vector<Bomb *> bomb_list;
    std::vector<Enemy *> enemy_list;

    /* Left, Up, Right, Down */
    const std::vector<int> dir_x {-1, 0, 1, 0};
    const std::vector<int> dir_y {0, 1, 0, -1};


public:
    Controller(Screen *scr, Level *lvl, Player *player);
    ~Controller();

    void update(double deltaT);

    void set_game_status(bool sts);
    bool get_game_status(void);

    Position move_player(Position new_pos);
    void kill_player(void);

    Position move_enemy(Enemy *enemy, Position new_pos);
    void kill_enemy(Position pos);

    int drop_bomb(Position pos, int remaining_time);
    void remove_bomb(Bomb *bomb);
    void explode_bomb(Bomb *bomb);
    Bomb *find_bomb(Position pos);

};


#endif

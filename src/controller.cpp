#include "controller.hpp"

Controller::Controller(Screen *scr, Level *level, Player *player)
{
    this->game_status = true;
    this->screen = scr;
    this->level = level;
    this->player = player;

    Enemy * enemy = new Enemy(std::make_tuple(1,13), std::make_tuple(1, 0), 100);
    Enemy * enemy_2 = new Enemy(std::make_tuple(11, 2), std::make_tuple(0, -1), 100);

    this->enemy_list.push_back(enemy);
    this->level->set_symbol(enemy->get_pos(), SYMBOL_ENEMY);

    this->enemy_list.push_back(enemy_2);
    this->level->set_symbol(enemy_2->get_pos(), SYMBOL_ENEMY);

}

int Controller::drop_bomb(Position pos, int remaining_time)
{
    Bomb *bomb;

    if (this->player->get_bomb_count() <= 0) {
        /* Not enough bombs stored */
        return 0;
    }

    /* Decrement player bomb count */
    this->player->remove_bomb();

    /* Include bomb on the bomb-list to be handled by the controller */
    bomb = new Bomb(pos, remaining_time, this->player->get_bomb_range());
    this->bomb_list.push_back(bomb);

    /* Set the bomb on the map */
    this->level->set_symbol(pos, SYMBOL_BOMB);

    /* Update the map with the bomb on screen */
    this->screen->update();

    return 1;
}

Position Controller::move_player(Position new_pos)
{
    Position old_pos = this->player->get_pos();

    Position ret_pos = old_pos;

    switch (this->level->get_symbol(new_pos)) {
        /* Should not reach here in single-player mode */
    case SYMBOL_PLAYER:
        /* For walls, bricks and bombs, don't move, since we can't get over them */
    case SYMBOL_WALL:
    case SYMBOL_BRICK:
    case SYMBOL_BOMB:
        break;

    case SYMBOL_SPACE:
        if (this->level->get_symbol(old_pos) != SYMBOL_BOMB) {
            this->level->set_symbol(old_pos, SYMBOL_SPACE);
        }
        this->level->set_symbol(new_pos, SYMBOL_PLAYER);
        this->player->set_pos(new_pos);
        ret_pos = new_pos;
        break;

    case SYMBOL_DOOR_FOUND:
        /* TODO: if there are no more enemies, finish level */
        break;

    case SYMBOL_ENEMY:
    case SYMBOL_EXPLOSION:
        this->kill_player();
        break;

    default:
        /* Invalid symbol - possible error */
        break;
    }
    /* Update the map on screen */
    this->screen->update();

    return ret_pos;
}

Position Controller::move_enemy(Enemy *enemy, Position new_pos)
{
    Velocity vel = enemy->get_velocity();

    Position old_pos = enemy->get_pos();
    Position ret_pos = old_pos;

    /* Check if the new x position is valid */
    switch (this->level->get_symbol(new_pos)) {
    case SYMBOL_PLAYER:
        /* Kill player */
	this->kill_player();
        break;

    case SYMBOL_ENEMY:
    case SYMBOL_SPACE:
        ret_pos = new_pos;
        this->level->set_symbol(old_pos, SYMBOL_SPACE);
        this->level->set_symbol(ret_pos, SYMBOL_ENEMY);
        enemy->set_pos(ret_pos);
        break;

    case SYMBOL_BRICK:
    case SYMBOL_WALL:
    case SYMBOL_DOOR_HIDDEN:
    case SYMBOL_DOOR_FOUND:
    case SYMBOL_BOMB:
        /* Reverse direction */
        std::get<0>(vel) *= -1;
        std::get<1>(vel) *= -1;
        enemy->set_velocity(vel);
        break;
    default:
        break;
    }

    /* Update the map on screen */
    this->screen->update();

    return ret_pos;
}


void Controller::update(double deltaT)
{
    /************ Bomb Handling ************/

    /* Update time for all bombs */
    for (std::vector<Bomb *>::iterator it = this->bomb_list.begin(); it != this->bomb_list.end(); it++) {
        (*it)->set_remaining_time((*it)->get_remaining_time() - deltaT);
    }

    /* Update bombs status */
    for (std::vector<Bomb *>::iterator it = this->bomb_list.begin(); it != this->bomb_list.end(); it++) {
        int rem_time = (*it)->get_remaining_time();
        if (rem_time <= 0) {
            explode_bomb(*it);
        } else if (rem_time <= 1000) {
            /* large bomb icon */
        } else if (rem_time <= 2000) {
            /* small bomb icon */
        }
    }

    /* Clean-up bombs */
    for (std::vector<Bomb *>::iterator it = this->bomb_list.begin(); it != this->bomb_list.end();) {
        if ((*it)->get_status() == BOMB_REMOVE) {
            delete (*it);
            it = this->bomb_list.erase(it);
            this->player->add_bomb();
        } else {
            it++;
        }
    }

    /************ Enemy Handling ************/
    for (std::vector<Enemy *>::iterator e = this->enemy_list.begin(); e != this->enemy_list.end(); e++) {
        Velocity vel;
        Position pos;

        vel = (*e)->get_velocity();
        pos = (*e)->get_pos();

        double pos_x = std::get<0>(pos);
        double pos_y = std::get<1>(pos);
        double vel_x = std::get<0>(vel);
        double vel_y = std::get<1>(vel);

        pos_x += (double)(vel_x * (deltaT/1000));
        pos_y += (double)(vel_y * (deltaT/1000));

        pos = this->move_enemy(*e, std::make_tuple(pos_x, pos_y));

    }

    move(2, 20);
    printw("Player Score: %d", this->player->get_score());
}

Bomb *Controller::find_bomb(Position f_pos)
{
    return *(std::find_if(this->bomb_list.begin(), this->bomb_list.end(),
                          [f_pos]( Bomb* const b) -> bool { return (b->get_pos() == f_pos); }));
}

void Controller::explode_bomb(Bomb *bomb)
{
    Position pos = bomb->get_pos();
    Position explosion_pos;
    int x = std::get<0>(pos);
    int y = std::get<1>(pos);
    int i,j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < bomb->get_range(); j++) {
            explosion_pos = std::make_tuple(x+(dir_x[i]*j), y+(dir_y[i]*j));

            switch (this->level->get_symbol(explosion_pos)) {
            case SYMBOL_PLAYER:
                /* Kill player */
                this->kill_player();
                break;

            case SYMBOL_WALL:
            case SYMBOL_DOOR_FOUND:
                /* Force the loop to go to the next explosion axis */
                j = bomb->get_range();
                break;

            case SYMBOL_BRICK:
                /* Destroy brick */
                if (bomb->get_status() == BOMB_ARMED) {
                    this->level->set_symbol(explosion_pos, SYMBOL_EXPLOSION);
                }
                j = bomb->get_range();
                break;

            case SYMBOL_DOOR_HIDDEN:
                this->level->set_symbol(explosion_pos, SYMBOL_DOOR_FOUND);
                j = bomb->get_range();
                break;

            case SYMBOL_BOMB:
                /* Explode bomb */
                if (explosion_pos == bomb->get_pos()) {
                    int symb = (bomb->get_status() == BOMB_EXPLODED) ? SYMBOL_SPACE : SYMBOL_EXPLOSION;
                    this->level->set_symbol(explosion_pos, symb);
                } else {
                    /* Force another bomb to explode */
                    this->explode_bomb(this->find_bomb(explosion_pos));
                }
                break;

            case SYMBOL_SPACE:
                if (bomb->get_status() == BOMB_ARMED) {
                    this->level->set_symbol(explosion_pos, SYMBOL_EXPLOSION);
                }
                break;

            case SYMBOL_EXPLOSION:
                if (bomb->get_status() == BOMB_EXPLODED) {
                    this->level->set_symbol(explosion_pos, SYMBOL_SPACE);
                }
                break;

            case SYMBOL_ENEMY:
                if (bomb->get_status() == BOMB_ARMED) {
                    this->level->set_symbol(explosion_pos, SYMBOL_EXPLOSION);
                    this->kill_enemy(explosion_pos);
                }
                j = bomb->get_range();
                break;
            default:
                break;
            }
        }
    }

    if (bomb->get_status() == BOMB_ARMED) {
        bomb->set_status(BOMB_EXPLODED);
        bomb->set_remaining_time(EXPLOSION_WEAROFF_TIME);
    } else if (bomb->get_status() == BOMB_EXPLODED){
        bomb->set_status(BOMB_REMOVE);
    }
}

void Controller::remove_bomb(Bomb *bomb)
{
    std::vector<Bomb *>::iterator bomb_vec_pos = std::find(this->bomb_list.begin(), this->bomb_list.end(), bomb);
    if (bomb_vec_pos != this->bomb_list.end()) {
        delete (*bomb_vec_pos);
        this->bomb_list.erase(bomb_vec_pos);
    }
}

void Controller::kill_enemy(Position pos)
{
    for (std::vector<Enemy *>::iterator it = this->enemy_list.begin(); it != this->enemy_list.end();) {
        double x = floor(std::get<0>((*it)->get_pos()));
        double y = floor(std::get<1>((*it)->get_pos()));
        if (std::make_tuple(x,y) == pos) {
            this->player->set_score(this->player->get_score() + (*it)->get_score());
            delete (*it);
            it = this->enemy_list.erase(it);
        } else {
            it++;
        }
    }
}

void Controller::kill_player(void)
{
    this->set_game_status(false);
}

bool Controller::get_game_status(void)
{
    return this->game_status;
}

void Controller::set_game_status(bool sts)
{
    this->game_status = sts;
}

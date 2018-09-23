#include "controller.hpp"

Controller::Controller(Screen *scr, Level *level, Player *player)
{
    this->screen = scr;
    this->level = level;
    this->player = player;
}

int Controller::drop_bomb(Position pos, int remaining_time, int range)
{
    Bomb *bomb;

    if (this->player->get_bomb_count() <= 0) {
        /* Not enough bombs stored */
        return 0;
    }

    /* Decrement player bomb count */
    this->player->remove_bomb();

    /* Include bomb on the bomb-list to be handled by the controller */
    bomb = new Bomb(pos, remaining_time, range);
    this->bomb_list.push_back(bomb);

    /* Set the bomb on the map */
    this->level->set_symbol(pos, SYMBOL_BOMB);

    /* Update the map with the bomb on screen */
    this->screen->update();

    return 1;
}

int Controller::is_colision(Position pos)
{
    Map *map = this->level->get_map();

    int x = std::get<0>(pos);
    int y = std::get<1>(pos);

    return ((*map)[x][y] != SYMBOL_SPACE);
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

    case SYMBOL_DOOR:
        /* TODO: if there are no more enemies, finish level */
        break;

    case SYMBOL_EXPLOSION:
        /* TODO: DIE DIE DIE DIE DIE */
        break;

    default:
        /* Invalid symbol - possible error */
        break;
    }
    /* Update the map on screen */
    this->screen->update();

    return ret_pos;
}

void Controller::update(int deltaT)
{
    /* Update time for all bombs */
    for (std::vector<Bomb *>::iterator it = this->bomb_list.begin(); it != this->bomb_list.end(); it++) {
        (*it)->set_remaining_time((*it)->get_remaining_time() - deltaT);
    }

    /* Update enemy list */

    /* Update bombs status */
    for (std::vector<Bomb *>::iterator it = this->bomb_list.begin(); it != this->bomb_list.end(); it++) {
	int rem_time = (*it)->get_remaining_time();
        if (rem_time <= 0) {
            /* explode */
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
                break;

            case SYMBOL_WALL:
		/* Force the loop to go to the next explosion axis */
		j = bomb->get_range();
		break;

            case SYMBOL_BRICK:
                /* Destroy brick */
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

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

    int bomb_cnt = this->player->get_bomb_count();

    if (bomb_cnt <= 0) {
	/* Not enough bombs stored */
	return 0;
    }

    /* Decrement player bomb count */
    this->player->set_bomb_count(bomb_cnt-1);

    /* Include bomb on the bomb-list to be handled by the controller */
    bomb = new Bomb(pos, remaining_time, range);
    this->bomb_list.push_back(bomb);

    /* Set the bomb on the map */
    this->level->set_bomb(pos);

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

    switch (this->level->get_type(new_pos)) {
	/* Should not reach here in single-player mode */
    case SYMBOL_PLAYER:
	/* For walls, bricks and bombs, don't move, since we can't get over them */
    case SYMBOL_WALL:
    case SYMBOL_BRICK:
    case SYMBOL_BOMB:
	break;

    case SYMBOL_SPACE:
	if (this->level->get_type(old_pos) != SYMBOL_BOMB) {
	    this->level->set_space(old_pos);
	}
	this->level->set_player(new_pos);
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

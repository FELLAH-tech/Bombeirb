/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <bomb.h>
#include <game.h>

struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int key;
	int life;
	int range;
};

struct player* player_init(int bombs) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bombs = bombs;
	player->key = 0;
	player->life = 9;
	player->range = 2;

	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}



int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

int player_get_nb_key(struct player* player) {
	assert(player);
	return player->key;
}

int player_get_nb_life(struct player* player) {
	assert(player);
	return player->life;
}

int player_inc_nb_life(struct player* player) {
	assert(player);
	return player->life +=1;

}
int player_dec_nb_life(struct player* player) {
	assert(player);
	return player->life -=1;
}

int player_get_nb_range(struct player* player) {
	assert(player);
	return player->range;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

static int player_move_aux(struct player* player,struct game** pgame, struct map* map, int x, int y) {
	struct game *game = *pgame;

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		if (map_get_cell(map,x,y) == CELL_PRINCESS_DOOR)
		{
			player->y-=1;
		}
		else if (map_get_cell(map,x,y) == CELL_PRINCESS){
				window_clear();
				window_display_image(sprite_get_win(), 0, 0);
				window_refresh();

				if(!start_game()){
					exit(1);
				}
				game_free(game);
				*pgame = game_new();
				game = *pgame;
				return 0;
		}
		else 
		{
			return 0;
		}
		break;
	case CELL_KEY:
		switch ((map_get_cell(map, x, y)& 0x0e)>>1)
		{
		case 0:
			map_set_cell_type(map, 1, 9, CELL_DOOR_OPENED);
			break;
		
		case 1:
			map_set_cell_type(map, 1, 0, CELL_DOOR_OPENED);
			break;
		case 2:
			map_set_cell_type(map, 11, 11, CELL_DOOR_OPENED);
			break;
		case 3:
			map_set_cell_type(map, 3, 0, CELL_DOOR_OPENED);
			break;
		case 4:
			map_set_cell_type(map, 4, 9, CELL_PRINCESS_DOOR);
			break;
		}
		player->key++;
		break;

	case CELL_DOOR:
		
		if ( (map_get_cell(map,x ,y)&0x01) == 0)
		{
			return 0;
		}
		else
		{
			if (player_get_x(player)%2 != 0) // condition pour determiner la porte du niveau supérieur 
			{
			level_inc(game);
			if ( player_get_nb_key(player) != 0)
			{
			player->key-- ;
			}
			}
			else if (player_get_x(player)%2 == 0) // condition pour determiner la porte du niveau inférieur
			{
			level_dec(game);
			}
			return 1;
		}


	case CELL_BOX:
		switch (player->direction) {
			case NORTH:
				if (!map_is_inside(map, x, y - 1) || map_get_cell_type(map ,x ,y - 1) == CELL_BOX || map_get_cell_type(map ,x ,y - 1) == CELL_SCENERY || map_get_cell_type(map ,x ,y - 1) == CELL_DOOR || map_get_cell_type(map ,x,y-1) == CELL_BONUS ) {
					return 0; 
				}
				break;

			case SOUTH:
				if (!map_is_inside(map, x, y + 1) || map_get_cell_type(map ,x ,y + 1) == CELL_BOX || map_get_cell_type(map ,x ,y + 1) == CELL_SCENERY || map_get_cell_type(map ,x ,y + 1) == CELL_DOOR || map_get_cell_type(map ,x,y+1) == CELL_BONUS) {
					return 0;
				}
				break;
			case WEST:
				if (!map_is_inside(map, x - 1, y) || map_get_cell_type(map ,x - 1,y) == CELL_BOX  || map_get_cell_type(map ,x - 1 ,y) == CELL_SCENERY || map_get_cell_type(map ,x - 1 ,y) == CELL_DOOR || map_get_cell_type(map ,x - 1 ,y) == CELL_BONUS ) {
					return 0;
				}
				break;

			case EAST:
				if (!map_is_inside(map, x + 1, y) || map_get_cell_type(map ,x + 1 ,y ) == CELL_BOX  || map_get_cell_type(map ,x + 1 ,y) == CELL_SCENERY || map_get_cell_type(map ,x + 1 ,y) == CELL_DOOR || map_get_cell_type(map ,x + 1 ,y) == CELL_BONUS ) {
					return 0;
				}
				break;
			
			default:	
				return 1;
				break;
		}

	case CELL_BONUS:
			switch (map_get_bonus_type(map,x,y) & 0x0f)
			{
			case BONUS_BOMB_NB_INC:
				player->bombs++;
				break;
			case BONUS_BOMB_NB_DEC:
				if ( player->bombs != 0)
				{
				player->bombs--;
				}
				break;
			case BONUS_LIFE:
				if(player->life > 0 && player->life < 8)
				{
					player->life++;
				}
				break;
			case BONUS_MONSTER:
				break;
			case BONUS_BOMB_RANGE_DEC:
				if(player->range > 1)
				{
					player->range--;
				}
				break;
			case BONUS_BOMB_RANGE_INC:
					player->range++;
				break;
			default:
				break;
			}
		break;

	case CELL_MONSTER:
			player->life--;
		break;

	case CELL_BOMB:
			if (map_get_bomb_type(map, x, y) == CELL_explosion)
			{
				player->life--;
			}
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player,struct game** pgame, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player,pgame, map, x, y - 1)) {
			player->y--;
			if (map_get_cell_type(map ,x ,y - 1) == CELL_BOX) {
				map_set_cell_type(map, x, y - 1, CELL_EMPTY);
				map_set_cell_type(map, x, y - 2, CELL_BOX);
			}
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player,pgame, map, x, y + 1)) {
			player->y++;
			if (map_get_cell_type(map ,x ,y + 1) == CELL_BOX) {
				map_set_cell_type(map, x, y + 1, CELL_EMPTY);
				map_set_cell_type(map, x, y + 2, CELL_BOX);
			}			
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player,pgame, map, x - 1, y)) {
			player->x--;
			if (map_get_cell_type(map ,x - 1 ,y) == CELL_BOX) {
				map_set_cell_type(map, x - 1, y, CELL_EMPTY);
				map_set_cell_type(map, x - 2, y, CELL_BOX);
			}			
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player,pgame, map, x + 1, y)) {
			player->x++;
			if (map_get_cell_type(map ,x + 1 ,y) == CELL_BOX) {
				map_set_cell_type(map, x + 1, y, CELL_EMPTY);
				map_set_cell_type(map, x + 2, y, CELL_BOX);
			}			
			move = 1;
		}
		break;
	}

	if (move && map_get_bomb_type(map ,x ,y) != CELL_Bomb4 && map_get_bomb_type(map, x, y) != CELL_Bomb3 && map_get_bomb_type(map, x, y) != CELL_Bomb2 && map_get_bomb_type(map, x, y) != CELL_Bomb1 && map_get_cell_type(map ,x ,y) != CELL_MONSTER) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}


void player_set_bomb(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int bom = player->bombs;
	if (bom != 0) {
		map_set_cell_type(map, x, y, CELL_Bomb4);
		player_dec_nb_bomb(player);
	}

}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}


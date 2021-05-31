/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <bomb.h>
#include <monster.h>
#include <game.h>


struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	if((x<0)||(y<0)||(y>map_get_height(map)-1)||(x>map_get_width(map)-1)){
		return 0;
	}
	return 1;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map);
	return map->grid[CELL(x,y)] & 0xf0;
}

enum compose_type map_get_bomb_type(struct map* map, int x, int y)
{
	assert(map);
	return map->grid[CELL(x,y)];
}

enum bonus_type map_get_bonus_type(struct map* map, int x, int y)
{
	assert(map);
	return map->grid[CELL(x,y)] & 0x0f;
}

unsigned char map_get_cell(struct map* map, int x, int y)
{
	assert(map);
	return map->grid[CELL(x,y)];
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map);
	map->grid[CELL(x,y)] = type;
}



void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_banner_life(), x, y);
		break;
	case BONUS_MONSTER:
		window_display_image(sprite_get_monster(SOUTH), x, y);
		break;
	}
}

void set_bonus_type(struct map *map,int x,int y,struct game *game)
{
	
	double n=(double) rand()/ (double) RAND_MAX;
	if(n<0.14){
		map_set_cell_type(map,x,y,CELL_BOX_BOMBDEC);
	}
	if((n>=0.14)&&(n<0.28)){
		map_set_cell_type(map,x,y,CELL_BOX_BOMBINC);
	}
	if((n>=0.28)&&(n<0.42)){
    	map_set_cell_type(map,x,y,CELL_BOX_RANGEDEC);
	}
	if((n>=0.42)&&(n<0.56)){
		map_set_cell_type(map,x,y,CELL_BOX_RANGEINC);
	}
	if((n>=0.56)&&(n<0.72)){
		map_set_cell_type(map,x,y,CELL_BOX_LIFE);
	}
	if((n>=0.72)&&(n<1)){
		game_set_monster(game);
		int b= monster_get_nbrmonster(game);
		struct monster * monster = game_get_monster(game,b-1);
		monster_set_position(monster,game_get_current_map(game),x,y);

	}

	if((n>=0.95)&&(n<1)){
		map_set_cell_type(map,x,y,CELL_EMPTY);
	}
	
}


void display_monster(struct map* map, int x, int  y, unsigned char type){
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
		case monster_north:
			window_display_image(sprite_get_monster(NORTH), x, y);
			break;
		case monster_south:
			window_display_image(sprite_get_monster(SOUTH), x, y);
			break;
		case monster_west:
			window_display_image(sprite_get_monster(WEST), x, y);
			break;
		case monster_east:
			window_display_image(sprite_get_monster(EAST), x, y);
			break;
	}
}

void display_bomb(struct map* map, int x, int  y, unsigned char type){
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
		case Bombe1:
			window_display_image(sprite_get_bomb(Bomm1), x, y);
			break;
		case Bombe2:
			window_display_image(sprite_get_bomb(Bomm2), x, y);
			break;
		case Bombe3:
			window_display_image(sprite_get_bomb(Bomm3), x, y);
			break;
		case Bombe4:
			window_display_image(sprite_get_bomb(Bomm4), x, y);
			break;
		case explosion:
			window_display_image(sprite_get_bomb(Bomm5), x, y);
			break;
	}
}



void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	case SCENERY_PRINCESS:
		window_display_image(sprite_get_princess(), x, y);
		break;

	case SCENERY_DOOR:
		window_display_image(sprite_get_door(), x, y);
		break;

	case SCENERY_CLOSED_DOOR:
		window_display_image(sprite_get_closed_door(), x, y);
		break;
	}
}

void display_door(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x01) { // Pour lire de dernier bit 0 pour fermé et 1 pour ouvert
	case 1:
		window_display_image(sprite_get_door_opened(), x, y);
		break;

	case 0:
		window_display_image(sprite_get_door_closed(), x, y);
		break;
	}
}


void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	
	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];
	    
	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;  
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
		case CELL_BOMB:
		  display_bomb(map, x, y ,type);
		  break;
	    case CELL_DOOR:
		  display_door(map,x, y, type);
	      break;
		case CELL_MONSTER:
			display_monster(map,x,y,type);
			break;
	    }
	  }
	}
}


struct map* get_map_numero(char N){
	
	
	char *name = malloc(11*sizeof(char));
	name[0]='m';
	name[1]='a';
	name[2]='p';
	name[3]='/';
	name[4]='m';
	name[5]='a';
	name[6]='p';
	name[7]='_';
	name[8]= N;
	name[9]='\0';

	FILE* file = NULL;

	file = fopen(name, "r+");


	int width,height;


	
	fscanf(file, "%d:%d" , &width,&height);

	struct map* map = map_new(width,height);

	// on charge la map avec les nombres qui se trouve dans le fichier map_N
	int i, j , k;
	for (i = 0; i < width; i++){
		  for (j = 0; j < height; j++){
			  fscanf(file, "%d" , &k);
			  map->grid[CELL(j,i)] = k;
		  }
	}

	fclose(file);
   	return map;
}

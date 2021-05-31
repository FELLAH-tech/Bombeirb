/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

enum cell_type {
	CELL_EMPTY=0x00,   		//  0000 0000
	CELL_SCENERY=0x10, 		//  0001 0000
	CELL_BOX=0x20,   		//  0010 0000
	CELL_DOOR=0x30,      	//  0011 0000
	CELL_KEY=0x40,       	//  0100 0000
	CELL_BONUS=0x50, 		// 	0101 0000
	CELL_MONSTER=0x60,		// 	0110 0000
	CELL_BOMB = 0x70, 	   		// 	0111 0000
};

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE
};

enum scenery_type {
	SCENERY_STONE       = 1,    // 0001 
	SCENERY_TREE        = 2,    // 0010 
	SCENERY_PRINCESS    = 4,  // 0100
	SCENERY_DOOR		= 5, // la porte de la princesse
	SCENERY_CLOSED_DOOR = 6,	 
};

enum direction_monster {
	monster_north,
	monster_south,
	monster_west,
	monster_east,

};

enum bomb_type{
	Bombe1,
	Bombe2,
	Bombe3,
	Bombe4,
	explosion,
};

enum door_type{
	DOOR_CLOSED = 0x00,
	DOOR_OPENED = 0x01,
};

enum compose_type {
	CELL_TREE     		      = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE   		 	  = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS 			  = CELL_SCENERY | SCENERY_PRINCESS,
	CELL_PRINCESS_DOOR 		  = CELL_SCENERY | SCENERY_DOOR,
	CELL_PRINCESS_CLOSED_DOOR = CELL_SCENERY | SCENERY_CLOSED_DOOR,

    CELL_BOX_RANGEINC = CELL_BONUS | BONUS_BOMB_RANGE_INC,
    CELL_BOX_RANGEDEC = CELL_BONUS | BONUS_BOMB_RANGE_DEC,
	CELL_BOX_BOMBINC  = CELL_BONUS | BONUS_BOMB_NB_INC,
    CELL_BOX_BOMBDEC  = CELL_BONUS | BONUS_BOMB_NB_DEC,
    CELL_BOX_LIFE     = CELL_BONUS | BONUS_LIFE,
    CELL_BOX_MONSTER  = CELL_BONUS | BONUS_MONSTER,


	CELL_MONSTER_NORTH = CELL_MONSTER | monster_north,
	CELL_MONSTER_SOUTH = CELL_MONSTER | monster_south,
	CELL_MONSTER_WEST  = CELL_MONSTER | monster_west,
	CELL_MONSTER_EAST  = CELL_MONSTER | monster_east,

	CELL_Bomb1      = CELL_BOMB | Bombe1 ,
	CELL_Bomb2      = CELL_BOMB | Bombe2 ,
	CELL_Bomb3		= CELL_BOMB | Bombe3 ,
	CELL_Bomb4 		= CELL_BOMB | Bombe4 ,
	CELL_explosion  = CELL_BOMB | explosion,

	CELL_DOOR_OPENED = CELL_DOOR | DOOR_OPENED,
	CELL_DOOR_CLOSED = CELL_DOOR | DOOR_CLOSED,
};


struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default static map
struct map* map_get_static();

// Display the map on the screen
void map_display(struct map* map);

//set the bonus on the cell of explosed box

//display the bonus in the explosed box
void display_bonus(struct map* map, int x, int y, unsigned char type);

// get bomb type
enum compose_type map_get_bomb_type(struct map* map, int x, int y);

//get bonus type
enum bonus_type map_get_bonus_type(struct map* map, int x, int y);

//get the cell
unsigned char map_get_cell(struct map* map, int x, int y);

struct map* get_map_numero(char N);


#endif /* MAP_H_ */

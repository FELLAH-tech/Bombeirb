/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>

// Sprites general
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR_OPENED	"sprite/door_opened.png"
#define MAP_DOOR_CLOSED	"sprite/door_closed.png"
#define MAP_DOOR		"sprite/door.png"
#define MAP_CLOSED_DOOR	"sprite/closed_door.png"

// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE	"sprite/banner_range.png"
#define DIGIT_0			"sprite/banner_0.jpg"
#define DIGIT_1			"sprite/banner_1.jpg"
#define DIGIT_2			"sprite/banner_2.jpg"
#define DIGIT_3			"sprite/banner_3.jpg"
#define DIGIT_4			"sprite/banner_4.jpg"
#define DIGIT_5			"sprite/banner_5.jpg"
#define DIGIT_6			"sprite/banner_6.jpg"
#define DIGIT_7			"sprite/banner_7.jpg"
#define DIGIT_8			"sprite/banner_8.jpg"
#define DIGIT_9			"sprite/banner_9.jpg"
#define PAUSE			"sprite/PAUse.png"

// Sprites of Bombs
#define BOMB_TTL1       "sprite/bomb1.png"
#define BOMB_TTL2       "sprite/bomb2.png"
#define BOMB_TTL3       "sprite/bomb3.png"
#define BOMB_TTL4       "sprite/bomb4.png"

//Sprites of explosion
#define EXPLOSION		"sprite/explosion.png"


// Menu 

#define MENU 		"sprite/start.jpg"
#define gameover	"sprite/gameover.jpg"
#define win 		"sprite/win.jpg"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

//Sprite of the princess
#define PRINCESS     "sprite/bomberwoman.png"

//Sprite of Levels
#define FLAG     "sprite/flag.png"


// Sprites of Monster 

#define MONSTER_LEFT     "sprite/monster_left.png"
#define MONSTER_UP       "sprite/monster_up.png"
#define MONSTER_RIGHT    "sprite/monster_right.png"
#define MONSTER_DOWN     "sprite/monster_down.png"


// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;

// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door_opened;
SDL_Surface* door_closed;
SDL_Surface* stone;
SDL_Surface* tree;
SDL_Surface* door;
SDL_Surface* closed_door;
SDL_Surface* princess;
SDL_Surface* pause;
SDL_Surface* flag;
SDL_Surface* menu;
SDL_Surface* wingame;
SDL_Surface* Gameover;



// bonus
#define NB_BONUS 4
SDL_Surface* bonus[NB_BONUS + 1];

// player
SDL_Surface* player_img[4];

// Monster

SDL_Surface* monster_img[4];

// bomb 

SDL_Surface* bomb_img[5];

static void banner_load() {
	// numbers imgs
	numbers[0] = image_load(DIGIT_0);
	numbers[1] = image_load(DIGIT_1);
	numbers[2] = image_load(DIGIT_2);
	numbers[3] = image_load(DIGIT_3);
	numbers[4] = image_load(DIGIT_4);
	numbers[5] = image_load(DIGIT_5);
	numbers[6] = image_load(DIGIT_6);
	numbers[7] = image_load(DIGIT_7);
	numbers[8] = image_load(DIGIT_8);
	numbers[9] = image_load(DIGIT_9);

	// other banner sprites
	banner_life = image_load(BANNER_LIFE);
	banner_bomb = image_load(BANNER_BOMB);
	banner_range = image_load(BANNER_RANGE);
	banner_line = image_load(BANNER_LINE);
}

static void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_line);
}

static void map_load() {
	// Sprite loading
	tree        = image_load(MAP_TREE);
	box         = image_load(MAP_CASE);
	key         = image_load(MAP_KEY);
	stone       = image_load(MAP_STONE);
	door_opened = image_load(MAP_DOOR_OPENED);
	door_closed = image_load(MAP_DOOR_CLOSED);
	door    = image_load(MAP_DOOR);
	closed_door    = image_load(MAP_CLOSED_DOOR);
	princess    = image_load(PRINCESS);
	pause		= image_load(PAUSE);
	flag		= image_load(FLAG);
	menu       = image_load(MENU);
	wingame		= image_load(win);
	Gameover	= image_load(gameover);
}

static void map_unload() {
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(door_opened);
	SDL_FreeSurface(door_closed);
	SDL_FreeSurface(door);
	SDL_FreeSurface(closed_door);
	SDL_FreeSurface(princess);
	SDL_FreeSurface(pause);
	SDL_FreeSurface(flag);
	SDL_FreeSurface(menu);
	SDL_FreeSurface(wingame);
	SDL_FreeSurface(Gameover);
}

static void bonus_load() {
	bonus[0] = NULL;
	bonus[BONUS_BOMB_RANGE_INC] = image_load(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_BOMB_RANGE_DEC] = image_load(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_NB_INC] = image_load(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_NB_DEC] = image_load(IMG_BONUS_BOMB_NB_DEC);
}

static void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		if(bonus[i])
			SDL_FreeSurface(bonus[i]);
}

static void player_load() {
	player_img[WEST] = image_load(PLAYER_LEFT);
	player_img[EAST] = image_load(PLAYER_RIGHT);
	player_img[NORTH] = image_load(PLAYER_UP);
	player_img[SOUTH] = image_load(PLAYER_DOWN);
}

static void monster_load() {
	monster_img[WEST] = image_load(MONSTER_LEFT);
	monster_img[EAST] = image_load(MONSTER_RIGHT);
	monster_img[NORTH] = image_load(MONSTER_UP);
	monster_img[SOUTH] = image_load(MONSTER_DOWN);
}

static void bomb_load() {
	bomb_img[Bomm1]= image_load(BOMB_TTL1);
	bomb_img[Bomm2]= image_load(BOMB_TTL2);
	bomb_img[Bomm3]= image_load(BOMB_TTL3);
	bomb_img[Bomm4]= image_load(BOMB_TTL4);
	bomb_img[Bomm5]= image_load(EXPLOSION);

}

static void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}

static void monster_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(monster_img[i]);
}

static void bomb_unload() {
	for (int i = 0; i < 5; i++)
		SDL_FreeSurface(bomb_img[i]);
}

void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	player_load();
	bomb_load();
	monster_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	bomb_unload();
	monster_unload();
}

SDL_Surface* sprite_get_number(short number) {
	//assert(number >= 0 && number < 10);
	return numbers[number];
}

SDL_Surface* sprite_get_player(enum direction direction) {
	assert(player_img[direction]);
	return player_img[direction];
}

SDL_Surface* sprite_get_monster(enum direction direction) {
	assert(monster_img[direction]);
	return monster_img[direction];
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type];
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_princess() {
	assert(princess);
	return princess;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_door_opened() {
	assert(door_opened);
	return door_opened;
}

SDL_Surface* sprite_get_door_closed() {
	assert(door_closed);
	return door_closed;
}

SDL_Surface* sprite_get_door() {
	assert(door);
	return door;
}

SDL_Surface* sprite_get_closed_door() {
	assert(closed_door);
	return closed_door;
}

SDL_Surface* sprite_get_bomb(enum bomba bomb) {
	assert(bomb_img[bomb]);
	return bomb_img[bomb];

}

SDL_Surface* sprite_get_pause() {
	assert(pause);
	return pause;
}

SDL_Surface* sprite_get_flag() {
	assert(flag);
	return flag;
}

SDL_Surface* sprite_get_menu() {
	assert(menu);
	return menu;
}

SDL_Surface* sprite_get_win() {
	assert(wingame);
	return wingame;
}

SDL_Surface* sprite_get_over() {
	assert(Gameover);
	return Gameover;
}


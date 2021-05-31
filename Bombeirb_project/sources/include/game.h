/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>


// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

int monster_get_nbrmonster(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

void level_inc(struct game* game);
void level_dec(struct game* game);

// game monster[a]
struct monster* game_get_monster(struct game* game,int a);

// add a new monster
void monster_add_nbrmonster(struct game* game);

// get num of level
short game_get_level(struct game* game);

z
// update
int game_update(struct game** game);

int pause_game();

// update des monstres
void monster_update(struct monster **monster, struct map* map, struct game* game);

int player_move(struct player* player,struct game** pgame, struct map* map);

#endif /* GAME_H_ */

/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

//ajouter un point de vie
int player_inc_nb_life(struct player* player);

//diminuer un point de vie
int player_dec_nb_life(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set the bomb in the position of the player
void player_set_bomb(struct player* player, struct map* map);
// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// get the number of key that the player has
int player_get_nb_key(struct player* player);

//get the number of life left
int player_get_nb_life(struct player* player);

//get number of range
int player_get_nb_range(struct player* player);



// Move the player according to the current direction
//int player_move(struct player* player,struct game* game, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

#endif /* PLAYER_H_ */

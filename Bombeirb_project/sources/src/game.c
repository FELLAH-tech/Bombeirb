/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <monster.h>
#include <player.h>
#include <map.h>

struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	int nb_monstre; // nombre des monsteres
	struct player* player;
	bomb_ancienne* bomb_ancienne;
	struct monster** monster;
};




struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	game->maps[0] = get_map_numero('0');
	game->maps[1] = get_map_numero('1');
	game->maps[2] = get_map_numero('2');
	game->maps[3] = get_map_numero('3');
	game->maps[4] = get_map_numero('4');

	game->levels = 5;
	game->level = 0;

	
	game->nb_monstre=1;
	game->monster = malloc(16*sizeof(struct monstre*));
	

	game->player = player_init(9);
	for(int i=0;i<game->nb_monstre;i++){
	    game->monster[i] = monster_init();
	}

	// Set default location of the player
	player_set_position(game->player, 1, 0);
	
	for(int i=0;i<game->nb_monstre;i++){
		monster_set_position(game->monster[i],game->maps[game->level], i+2,0);
	}
	
	game->bomb_ancienne = initialisation();
	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);


	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
	for (int i = 0; i < game->nb_monstre; i++)
		monster_free(game->monster[i]);
}

int monster_get_nbrmonster(struct game* game){
	assert(game);
	return game->nb_monstre;
}

short game_get_level(struct game* game){
	assert(game);
	return game->level;
}

void monster_add_nbrmonster(struct game* game){
	assert(game);
	(game->nb_monstre)+= 1;
}

void level_inc(struct game* game){
	assert(game);
	(game->level)++;
}

void level_dec(struct game* game){
	assert(game);
	(game->level)--;
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}

void game_set_monster(struct game *game){
	int n=monster_get_nbrmonster(game);
	game->monster[n] = monster_init();
	monster_add_nbrmonster(game);

}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct monster* game_get_monster(struct game* game,int a) {
	assert(game);
	return game->monster[a];
}


bomb_ancienne* game_get_bomb_ancienne(struct game* game) {
	assert(game);
	return game->bomb_ancienne;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 8 * SIZE_BLOC) / 9;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_range(game_get_player(game))), x, y);
	
	x = 4 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_key(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_key(game_get_player(game))), x, y);

	x = 4 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_key(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_key(game_get_player(game))), x, y);

	x = 4 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_flag(), x, y);

	x = 4 * white_bloc + 9 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(game_get_level(game)), x, y);
}

void game_display(struct game* game) {
	assert(game);

	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);



	window_refresh();
}

static short input_keyboard(struct game** pgame) {
	struct game *game = *pgame;
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	int done=0;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_q:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player,pgame, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player,pgame, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player,pgame, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player,pgame, map);
				break;
			case SDLK_SPACE:{
				int k = player_get_nb_bomb(player);
				if ( k!=0)
				{
				bomb_ancienne* bomb_ancienne = game_get_bomb_ancienne(game);
				insertion(bomb_ancienne,player);
				player_set_bomb(player, map);
				}
				break;}
			case SDLK_p:
				window_display_image(sprite_get_pause(), 3*40, 3*40);
				window_refresh();
				while(!done){
					done = pause_game();
				}
			case SDLK_s:
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}


int pause_game(){
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_p:
				return 1;
				break;
			default:
				break;
			}
		break;
		}
	}
	return 0;
}

int start_game(){
	SDL_Event event;
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;
	while (1){
		timer = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					return 0;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_s:
							return 1;
							break;
						case SDLK_q:
							return 0;
							break;
						case SDLK_ESCAPE:
							return 0;
							break;
						default:
							break;
						}
				default:
					break;
			}
		}

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}
}

int game_update(struct game** pgame) {

	struct game *game = *pgame;

	if(player_get_nb_life((game)->player)==-1){
		window_clear();
		window_display_image(sprite_get_over(), 0, 0);
		window_refresh();
		if(!start_game()){
			exit(1);
		}
		game_free(game);
		*pgame = game_new();
		game = *pgame;
	}

	struct map* map = game_get_current_map(game);
	bomb_ancienne* bomb_ancienne = game_get_bomb_ancienne(game);
	struct player* player = game_get_player(game);



	if (input_keyboard(pgame))
		return 1; // exit game
	
	Bomb_update_global(bomb_ancienne,map,player,game);

	monster_update(game->monster,map,game);

	return 0;
}

/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <sprite.h>


int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	struct game* game = game_new();

	window_create(SIZE_BLOC * STATIC_MAP_WIDTH,
	SIZE_BLOC * STATIC_MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	// game loop
	// static time rate implementation
	int done = 0;
	char menu_displayed=1;
	SDL_Event event;

	while (!done) {
		timer = SDL_GetTicks();
		if(menu_displayed){
			window_clear();
			window_display_image(sprite_get_menu(),0,0);
			window_refresh();

			while(SDL_PollEvent(&event)){
				switch (event.type)
				{
				case SDL_QUIT:
					done=1;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_q:
							done=1;
							break;
						case SDLK_ESCAPE:
							done=1;
							break;
						case SDLK_s:
							menu_displayed=0;
							break;
						/*
						case SDLK_l:
							game_free(game);
							game = game_load();
							break;
						*/
						default:
							break;
					}
				default:
					break;
				}
			}

		}

		else{
			done = game_update(&game);
			game_display(game);
		}
		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	
	}

	game_free(game);

	SDL_Quit();

	return EXIT_SUCCESS;
}




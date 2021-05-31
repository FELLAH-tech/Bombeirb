#include <SDL/SDL_image.h>
#include <assert.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <game.h>


struct monster {
	int x, y;
    enum direction direction;
 };


struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->x=0;
	monster->y=0;

    monster->direction=NORTH;

	return monster;
}

void monster_set_position(struct monster *monster,struct map* map ,int x, int y) {
	assert(monster);

	map_set_cell_type(map,monster->x,monster->y,CELL_EMPTY);
	monster->x = x;
	monster->y = y;
	map_set_cell_type(map,x,y,CELL_MONSTER);


}



void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction dirrection) {
	assert(monster);
	monster->direction = dirrection;
}


static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y)||map_get_cell_type(map, x, y)==CELL_DOOR||map_get_cell_type(map, x, y)==CELL_SCENERY||map_get_cell_type(map, x, y)==CELL_KEY||map_get_cell_type(map, x, y)==CELL_BOX){
		return 0;
	}
	else if (map_get_cell_type(map, x+1, y)==CELL_DOOR || map_get_cell_type(map, x-1, y)==CELL_DOOR || map_get_cell_type(map, x, y+1)==CELL_DOOR || map_get_cell_type(map, x, y-1)==CELL_DOOR)
		{
			return 0;
		}
	if (map_get_cell_type(map, x, y)==CELL_EMPTY){
		return 1;
	}



	return 0;
}

int monster_move(struct monster* monster, struct map* map) {

	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1)){
			map_set_cell_type(map,x,y-1,CELL_MONSTER_NORTH);
			monster->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x , y + 1)){
			map_set_cell_type(map,x,y+1,CELL_MONSTER_SOUTH);
			monster->y++;
			move=1;
			}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1 , y )){
			map_set_cell_type(map,x-1,y,CELL_MONSTER_WEST);
			monster->x--;
			move=1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1 , y )){
			map_set_cell_type(map,x+1,y,CELL_MONSTER_EAST);
			monster->x++;
			move=1;
		}
		break;
	}
	if (move)
		map_set_cell_type(map,x,y,CELL_EMPTY);
	return move;
}

enum direction update_monster_direction(struct monster * monster){
	double n;
	n=(double) rand()/ (double) RAND_MAX;
	if(n<0.10){
		return NORTH;
		}
	if((n>=0.10)&&(n<0.3)){
		return SOUTH;
	}
	if((n>=0.3)&&(n<0.8)){	
		return EAST;
	}
	if((n>=0.8)&&(n<1)){
		return WEST;
	}
	return NORTH;
}

int get_vitesse(struct game * game){
	switch (game_get_level(game))
		{
		case 0:
			return vitesse1;
		case 1:
			return vitesse2;
		case 2:
			return vitesse3;
		case 3:
			return vitesse3;
		case 4:
			return vitesse4;
		default:
			break;
		}
	return vitesse1;
}

void monster_update(struct monster **monster, struct map* map, struct game* game)
{
	//double n;

	int temps = SDL_GetTicks();
	int Nbr=monster_get_nbrmonster(game);
	int vitesse=get_vitesse(game);
	

	if(temps%vitesse==0){
		for(int i=0;i<Nbr;i++){
			monster_set_current_way(monster[i],update_monster_direction(monster[i]));
			monster_move(monster[i],map);
		}
	}
}


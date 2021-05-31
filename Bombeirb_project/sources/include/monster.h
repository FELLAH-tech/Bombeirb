#include <map.h>
#include <constant.h>

#define vitesse1 16
#define vitesse2 12
#define vitesse3 8
#define vitesse4 4


struct monster;

// Creer un monster et lui associer une direction
struct monster* monster_init();

void   monster_free(struct monster* monster);

// associer au monstre une position
void monster_set_position(struct monster *monster,struct map* map ,int x, int y);

// retourner les positions du monster
int monster_get_x(struct monster* monster);

int monster_get_y(struct monster* monster);

// retourner la direction du deplacement du monster 
void monster_set_current_way(struct monster * monster, enum direction direction);

// déplacement du monster selon une direction
int monster_move(struct monster* monster, struct map* map);

//display the monster on the screen
void display_monster(struct map* map, int x, int  y, unsigned char type);





#include <map.h>
#include <constant.h>

#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>
#include <map.h>

// c'est la file des bombes 
struct Element_bomb;

// pointeur vers la dernière bombe posée
typedef struct bomb_ancienne bomb_ancienne;
struct bomb_ancienne;

bomb_ancienne *initialisation();

//ajouter une nouvelle bombe dans la file
void insertion(bomb_ancienne *bomb_ancienne, struct player* player);

//supprimer la bombe explosée de la file
void supprimer(bomb_ancienne *bomb_ancienne);

//Parcourir la file des bombes pour les mettre à jour
int Bomb_update_global(bomb_ancienne *bomb_ancienne,struct map *map, struct player *player,struct game * game);

//mettre à jour l'état d'une bombe
int bomb_update(struct Element_bomb* Element_bomb,struct map *map, int temps_ref, struct player *player,struct bomb_ancienne* bomb_ancienne,struct game * game);

//determiner la position de la bombe depuis la position du player
int bomb_set_pos_x(struct Element_bomb* Element_bomb,struct player *player);

//determiner la position de la bombe depuis la position du player
int bomb_set_pos_y(struct Element_bomb* Element_bomb,struct player *player);

//permettre de sauvegarder le dernièr état de la bomb pour parcourir les cases
void player_set_current_bomb(struct Element_bomb* Element_bomb, enum compose_type current_bomb);

void set_bonus_type(struct map *map,int x,int y,struct game *game);



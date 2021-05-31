
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>
#include <map.h>
#include <bomb.h>


typedef struct Element_bomb Element_bomb;
struct Element_bomb
{
    enum bomb_type type_bomb;
	int temps_ref; // chaque bombe aura son temps de début de son déclanchement
	int x,y;
    Element_bomb *suivant;
	
};

typedef struct bomb_ancienne bomb_ancienne;
struct bomb_ancienne
{
    Element_bomb *premier;
};

bomb_ancienne *initialisation()
{
    bomb_ancienne *bomb_ancienne = malloc(sizeof(*bomb_ancienne));

    if (bomb_ancienne == NULL)
    {
        exit(EXIT_FAILURE);
    }

    bomb_ancienne->premier = NULL;

    return bomb_ancienne;
}

void insertion(bomb_ancienne *bomb_ancienne, struct player* player) // une file de bombe
{
    Element_bomb *nouveau_bomb = malloc(sizeof(*nouveau_bomb));
    if (nouveau_bomb == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau_bomb->type_bomb = Bombe4;
	nouveau_bomb->temps_ref = SDL_GetTicks(); // l'instant du lancement de la bombe
    nouveau_bomb->suivant = NULL;
    nouveau_bomb->x = bomb_set_pos_x(nouveau_bomb,player);
    nouveau_bomb->y = bomb_set_pos_y(nouveau_bomb,player);

    if (bomb_ancienne->premier != NULL) /* La file contient au moins deux */
    {
        /* On se positionne à la fin de la file */
        Element_bomb *Actuelle_bomb = bomb_ancienne->premier;
        while (Actuelle_bomb->suivant != NULL)
        {
            Actuelle_bomb = Actuelle_bomb->suivant;
        }		
        Actuelle_bomb->suivant = nouveau_bomb;
    }
    else /* la file est vide */
    {
        bomb_ancienne->premier = nouveau_bomb;
    }
}

void supprimer(bomb_ancienne *bomb_ancienne)
{
    assert(bomb_ancienne);

    /* On vérifie s'il y a quelque chose à défiler */
    if (bomb_ancienne->premier != NULL)
    {
        Element_bomb *bombe_a_suprimee = bomb_ancienne->premier;
        bomb_ancienne->premier = bombe_a_suprimee->suivant;
        free(bombe_a_suprimee);
    }

}

int Bomb_update_global(bomb_ancienne *bomb_ancienne,struct map *map, struct player *player,struct game* game)
{
    if (bomb_ancienne->premier == NULL) // pour éviter l'éxécution de la fonction avant le 1er espace
    {
        return 0;
    }
	else
	{
        Element_bomb *actuelle_bomb = bomb_ancienne->premier;

        while (actuelle_bomb != NULL) // parcourir la file des bombes
        {	
            bomb_update(actuelle_bomb,map,actuelle_bomb->temps_ref,player,bomb_ancienne,game);
            actuelle_bomb = actuelle_bomb->suivant;
        }
        return 0;
	}
}


int bomb_set_pos_x(struct Element_bomb* Element_bomb,struct player *player)
{
	Element_bomb->x = player_get_x(player);
    return Element_bomb->x;
}

int bomb_set_pos_y(struct Element_bomb* Element_bomb,struct player *player)
{
	Element_bomb->y = player_get_y(player);
    return Element_bomb->y;
}

void player_set_current_bomb(struct Element_bomb* Element_bomb, enum compose_type current_bomb)
{
	assert(Element_bomb);
	Element_bomb->type_bomb = current_bomb;
}




int bomb_update(struct Element_bomb* Element_bomb,struct map *map, int temps_ref, struct player *player,struct bomb_ancienne* bomb_ancienne,struct game *game)
{

		int x = Element_bomb->x;
		int y = Element_bomb->y;
		int temps_apres = SDL_GetTicks(); //mise à jours du temps à chaque nouveau update
        
        int range = player_get_nb_range(player);
        static enum cell_type cell_type[12][4]; //tableau qui change de valeur s'il y a un box pour activer son explosion dans le case explosion
		switch (Element_bomb->type_bomb)
		{
		case Bombe4:
			if (temps_apres - temps_ref > 1000 && temps_apres - temps_ref < 2000 )
			{
			player_set_current_bomb(Element_bomb,Bombe3);
			map_set_cell_type(map, x, y, CELL_Bomb3);
			
			}
			break;

		case Bombe3:
			if (temps_apres - temps_ref > 2000 && temps_apres - temps_ref < 3000 )
			{
			player_set_current_bomb(Element_bomb,Bombe2);
			map_set_cell_type(map, x, y, CELL_Bomb2);

			}
			break;	

		case Bombe2:
			if (temps_apres - temps_ref > 3000 && temps_apres - temps_ref < 4000 )
			{
			player_set_current_bomb(Element_bomb,Bombe1);
			map_set_cell_type(map, x, y, CELL_Bomb1);

			}
			break;

		case Bombe1:
			if (temps_apres - temps_ref > 4000 && temps_apres - temps_ref < 5000 )
            {
                player_set_current_bomb(Element_bomb,explosion);            
                map_set_cell_type(map, x, y, CELL_explosion);
                player_inc_nb_bomb(player);
                
                //pour s'assurer à la prochaine itération qu'il n'y a pas de propagation d'explosion
                int move=0;
                int move2=0;
                int move3=0;
                int move4=0;

                for (int i=1; i<range+1;i++) // gestion de la portee
                {

                    if (map_is_inside(map,x,y+i))
                    {    
                            if ( map_get_cell_type(map,x,y+i) == CELL_BOX )
                            {  
                                
                                if(move==0){
                                   cell_type[i][0] = map_get_cell_type(map,x,y+i);
                                   map_set_cell_type(map, x, y+i, CELL_explosion);
                                   move++;
                                }
                                
                            }
                            else if(map_get_cell_type(map,x,y+i) == CELL_SCENERY || map_get_cell_type(map,x,y+i) == CELL_KEY || map_get_cell_type(map,x,y+i) == CELL_DOOR)
                            {
                                break;
                            }
                            else {
                              map_set_cell_type(map, x, y+i, CELL_explosion);  
                            }

                    }
                }
                for (int i=1; i<range+1;i++){
                    if (map_is_inside(map,x,y-i))
                    {
                            if ( map_get_cell_type(map,x,y-i) == CELL_BOX)
                            {
                            
                                if ( move2 == 0)
                                {
                                   cell_type[i][1] = map_get_cell_type(map,x,y-i);
                                   map_set_cell_type(map, x, y-i, CELL_explosion);
                                   move2++;
                                }
                            }
                            else if (map_get_cell_type(map,x,y-i) == CELL_SCENERY || map_get_cell_type(map,x,y-i) == CELL_KEY || map_get_cell_type(map,x,y-i) == CELL_DOOR)
                                {
                                    break;
                                }
                            else{
                              map_set_cell_type(map, x, y-i, CELL_explosion);  
                            }
                    }
                    
                }  
                for (int i=1; i<range+1;i++){
                    if (map_is_inside(map,x+i,y))
                    {
 
                            if ( map_get_cell_type(map,x+i,y) == CELL_BOX)
                            {
                                if ( move3 == 0)
                                {
                                   cell_type[i][2] = map_get_cell_type(map,x+i,y);
                                   map_set_cell_type(map, x+i, y, CELL_explosion);
                                   move3++;
                                }
                            }
                            else if (map_get_cell_type(map,x+i,y) == CELL_SCENERY || map_get_cell_type(map,x+i,y) == CELL_KEY || map_get_cell_type(map,x+i,y) == CELL_DOOR)
                                {
                                    break;
                                }
                            else {
                                map_set_cell_type(map, x+i, y, CELL_explosion);
                            
                                }
                        }
                }

                for (int i=1; i<range+1;i++){
                    if (map_is_inside(map,x-i,y))
                    {
                            if ( map_get_cell_type(map,x-i,y) == CELL_BOX)  
                            {
                                if ( move4 == 0)
                                {
                                   cell_type[i][3] = map_get_cell_type(map,x-i,y);
                                   map_set_cell_type(map, x-i, y, CELL_explosion);
                                   move4++;
                                }
                            }
                            else if (map_get_cell_type(map,x-i,y) == CELL_SCENERY || map_get_cell_type(map,x-i,y) == CELL_KEY || map_get_cell_type(map,x-i,y) == CELL_DOOR)
                                {
                                    break;
                                }
                            else {
                              map_set_cell_type(map, x-i, y, CELL_explosion);  
                            }
                    }
                }			
            }
			break;
		case explosion:
			if (temps_apres - temps_ref > 5000 && temps_apres - temps_ref < 6000 )
			{
                map_set_cell_type(map, x, y, CELL_EMPTY);
                for (int i=1; i<range+1;i++)
                {
                    
                    if (map_is_inside(map,x,y+i))
                    {

                            if ( cell_type[i][0] == CELL_BOX)
                            {
                                set_bonus_type(map,x,y+i,game);
                                cell_type[i][0] = CELL_EMPTY;
                            }
                            else if (map_get_cell_type(map,x,y+i) == CELL_SCENERY || map_get_cell_type(map,x,y+i) == CELL_KEY || map_get_cell_type(map,x,y+i) == CELL_DOOR)
                            {
                                break;
                            }
                            else if (map_get_cell_type(map,x,y+i) != CELL_BONUS && map_get_cell_type(map,x,y+i) != CELL_BOX)
                            {
                                if (player_get_x(player) == x && player_get_y(player) == y+i)
                                {
                                    player_dec_nb_life(player);
                                }                                
                                map_set_cell_type(map, x, y+i, CELL_EMPTY);
                            }
                    }
                }

                for (int i=1; i<range+1;i++){
                    if (map_is_inside(map,x,y-i))
                    {
                        if ( cell_type[i][1] == CELL_BOX)
                        {
                            set_bonus_type(map,x,y-i,game);
                            cell_type[i][1] = CELL_EMPTY;
                        }
                        else if (map_get_cell_type(map,x,y-i) == CELL_SCENERY || map_get_cell_type(map,x,y-i) == CELL_KEY || map_get_cell_type(map,x,y-i) == CELL_DOOR)
                        {
                            break;
                        }
                        else if(map_get_cell_type(map,x,y-i) != CELL_BONUS && map_get_cell_type(map,x,y-i) != CELL_BOX)
                        {
                                if (player_get_x(player) == x && player_get_y(player) == y-i)
                                {
                                    player_dec_nb_life(player);
                                }                                
                            map_set_cell_type(map, x, y-i, CELL_EMPTY);
                        }
                        
                     }
                }
                for (int i=1; i<range+1;i++){
                    if (map_is_inside(map,x+i,y))
                    {
                        if ( cell_type[i][2] == CELL_BOX)
                        {
                            set_bonus_type(map,x+i,y,game);
                            cell_type[i][2] = CELL_EMPTY;
                        }
                        else if (map_get_cell_type(map,x+i,y) == CELL_SCENERY ||map_get_cell_type(map,x+i,y) == CELL_KEY || map_get_cell_type(map,x+i,y) == CELL_DOOR)
                        {
                            break;
                        }
                        else if(map_get_cell_type(map,x+i,y) != CELL_BONUS && map_get_cell_type(map,x+i,y) != CELL_BOX)
                        {
                                if (player_get_x(player) == x+i && player_get_y(player) == y)
                                {
                                    player_dec_nb_life(player);
                                }                                
                            map_set_cell_type(map, x+i, y, CELL_EMPTY);
                        }
                    }                        
                }
                for (int i=1; i<range+1;i++){
                    if (map_is_inside(map,x-i,y))
                    {
                        if ( cell_type[i][3] == CELL_BOX)
                        {
                            set_bonus_type(map,x-i,y,game);
                            cell_type[i][3] = CELL_EMPTY;
                        }
                        else if (map_get_cell_type(map,x-i,y) == CELL_SCENERY || map_get_cell_type(map,x-i,y) == CELL_KEY || map_get_cell_type(map,x-i,y) == CELL_DOOR)
                        {
                            break;
                        }
                        else if(map_get_cell_type(map,x-i,y) != CELL_BONUS && map_get_cell_type(map,x-i,y) != CELL_BOX)
                        {
                                if (player_get_x(player) == x-i && player_get_y(player) == y)
                                {
                                    player_dec_nb_life(player);
                                }                                
                            map_set_cell_type(map, x-i, y, CELL_EMPTY);
                        }
                    }
                }
            supprimer(bomb_ancienne); 
            }
			break;

		default:
			break;
        }
		return 1;
	
}





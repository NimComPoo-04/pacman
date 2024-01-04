#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SDL2/SDL.h>
#include "map.h"

typedef struct
{
	enum { UP, DOWN, LEFT, RIGHT } direction;

	int allow_door;

	// Defines the center of the entity
	int x; int y;
	int w; int h;

	int vx; int vy;
} entity_t;

// moves if a move is possible
int entity_move(entity_t *e, map_t *m);

// checks for entity entity collision
int entity_collision_entity(entity_t *e1, entity_t *e2);

// checks for map entity collision
int entity_collision_map(entity_t *e, map_t *m, int type);

void entity_debug_draw(entity_t *e, SDL_Renderer *r, uint32_t col);

#endif

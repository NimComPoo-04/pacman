#ifndef _GHOST_H_
#define _GHOST_H_

#include <SDL2/SDL.h>
#include "entity.h"
#include "pacman.h"
#include "map.h"

enum
{
	START,
	SCATTER,
	CHASE,
	FRIGHTENTED,
	DEAD
};

typedef struct ghost_t
{
	entity_t entity;
	int state;
	
	int target_x;
	int target_y;

	pacman_t *pacman;

	int frighten_timer;
	int scatter_timer;
	int chase_timer;

	void(*target_reassign)(struct ghost_t *, map_t *map, void *);
} ghost_t;

// changes state
int ghost_update(ghost_t *g, map_t *map, Uint32 frames, void *v);
void ghost_debug_draw(ghost_t *g, SDL_Renderer *ren);

void ghost_draw(ghost_t *g, SDL_Renderer *ren, Uint32 color);

// Send blinky when initing clyde
enum
{
	BLINKY,
	PINKY,
	INKY,
	CLYDE
};

ghost_t ghost_create(pacman_t *p, map_t *m, int type);

#endif

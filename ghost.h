#ifndef _GHOST_H_
#define _GHOST_H_

#include <SDL2/SDL.h>
#include "map.h"
#include "pacman.h"

typedef struct
{
	enum {
		BLINKY,	// red
		INKY,	// cyan
		PINKY,  // pink
		CLYDE   // orange
	}type;

	enum {
		SCATTER, // next 15 seconds then follow 
		FOLLOW, // next 45 seconds then scatter
		PANIC,	 // when pacman eats big, 10 seconds
	} state;

	int x; int y;
	int w; int h;

	int vx;
	int vy;

	int from_mx;
	int from_my;

	int target_mx;
	int target_my;

	map_t *map;

	char *visited_nodes;

	char stack[1024];
	int  top;
} ghost_t;

void ghost_update_target(ghost_t *g, int x, int y);
int ghost_add_move(ghost_t *g, char move);
int ghost_get_move(ghost_t *g);
int ghost_drop_stack(ghost_t *g);
void ghost_generate_path(ghost_t *g);

void ghost_update(ghost_t *g, pacman_t *p, int ticks);
void ghost_draw(ghost_t *g, SDL_Renderer *renderer);

#endif

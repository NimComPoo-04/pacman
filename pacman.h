#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "SDL2/SDL.h"
#include "map.h"

typedef struct
{
	int x; int y;
	int w; int h;

	int vx; int vy;

	int score;

	enum { UP, DOWN, LEFT, RIGHT } direction;
} pacman_t;

int pacman_move(pacman_t *p, map_t *m);
void pacman_draw(pacman_t *p, SDL_Renderer *r, int time);
void pacman_change_dir(pacman_t *p, int d, int v);
void pacman_eat(pacman_t *p, map_t *m);

#endif


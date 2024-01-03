#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "SDL2/SDL.h"
#include "map.h"
#include "entity.h"

typedef struct {
	entity_t entity;
	int score;
} pacman_t;

pacman_t pacman_create_default(int, int);
void pacman_draw(pacman_t *p, SDL_Renderer *r, int time);
int pacman_eat(pacman_t *p, map_t *m);

#endif


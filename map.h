#ifndef _MAP_H_
#define _MAP_H_

#include <SDL2/SDL.h>

enum
{
	NORMAL_FOOD = '.',
	BIG_FOOD = 'o',
	RED_FOOD = 'x',
	NONE = ' ',
	DOOR = '_',
	WALL = '*',
};

typedef struct
{
	int width;
	int height;
	int cell_width;
	int cell_height;
	int food_count;
	char *buffer;
} map_t;

map_t map_create_default(int width, int height);
void map_draw(map_t *map, SDL_Renderer *renderer);

// Afully sus :) Why am i doing this? I am lazy. Will it come back to haunt me, YES.
extern int(*map_pos_at)(map_t *m, int x, int y);

#endif

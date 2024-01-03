#include <time.h>
#include <stdlib.h>
#include "map.h"

static char def_map_map_buffer[] = 
		"****************************"
		"*............**............*"
		"*.****.*****.**.*****.****.*"
		"*o****.*****.**.*****.****o*"
		"*.****.*****.**.*****.****.*"
		"*..........................*"
		"*.****.**.********.**.****.*"
		"*.****.**.********.**.****.*"
		"*......**....**....**......*"
		"******.***** ** *****.******"
		"     *.***** ** *****.*     "
		"     *.**          **.*     "
		"     *.** ***__*** **.*     "
		"******.** **    ** **.******"
		"      .   **    **   .      "
		"******.** ******** **.******"
		"     *.** ******** **.*     "
		"     *.**          **.*     "
		"     *.** ******** **.*     "
		"******.** ******** **.******"
		"*............**............*"
		"*.****.*****.**.*****.****.*"
		"*.****.*****.**.*****.****.*"
		"*o..**................**..o*"
		"***.**.**.********.**.**.***"
		"***.**.**.********.**.**.***"
		"*......**....**....**......*"
		"*.**********.**.**********.*"
		"*.**********.**.**********.*"
		"*..........................*"
		"****************************";

static const map_t def_map = {
	.width = 28,
	.height = 31,
	.cell_width = 0,
	.cell_height = 0,
	.food_count = 246,
	.buffer = 0
};

static int at(map_t *m, int x, int y)
{
	if(x >= m->width || x < 0) return NONE;
	if(y >= m->height || y < 0) return NONE;

	return m->buffer[y * m->width + x];
}

int(*map_pos_at)(map_t *m, int x, int y);

map_t map_create_default(int width, int height)
{
	srand(time(NULL));
	map_t m = def_map;
	m.cell_width = width / m.width;
	m.cell_height = height / m.height;

	map_pos_at = at;

	if(m.buffer == NULL)
		m.buffer = calloc(sizeof(char), m.width * m.height);
	memcpy(m.buffer, def_map_map_buffer, sizeof def_map_map_buffer);

	int x = 0, y = 0;
	for(int i = 0; i < 6; i++)
	{
		x = (rand() % m.width);
		y = (rand() % m.height);

		if(m.buffer[y * m.width + x] == NORMAL_FOOD)
			m.buffer[y * m.width + x] = BIG_FOOD;
	}

	return m;
}

void map_draw(map_t *map, SDL_Renderer *render, uint32_t color)
{
	const int lw = map->cell_width;
	const int lh = map->cell_height;

	const int ldw = lw * 10 / 29;
	const int ldh = lh * 10 / 29;

	// SDL_Rect r;

	for(int x = 0; x < map->width; x++)
	{
		for(int y = 0; y < map->height; y++)
		{
			const int lx = x * lw;
			const int ly = y * lh;

			// r.x = lx; r.y = ly;
			// r.w = lw; r.h = lh;

			switch(at(map, x, y))
			{
				case NONE:
					break;


				case WALL:
					{
						SDL_SetRenderDrawColor(render, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 0);

						int c = 1;

						if(at(map, x + 1, y) != WALL)
						{
							SDL_RenderDrawLine(render,
									lx + lw - ldw,
									ly + ldh * (at(map, x, y - 1) != WALL) ,
									lx + lw - ldw,
									ly + lh - ldh * (at(map, x, y + 1) != WALL));
						}
						else
							c *= 2;

						if(at(map, x - 1, y) != WALL)
						{
							SDL_RenderDrawLine(render,
									lx + ldw,
									ly + ldh * (at(map, x, y - 1) != WALL),
									lx + ldw,
									ly + lh - ldh * (at(map, x, y + 1) != WALL));
						}
						else
							c *= 3;

						if(at(map, x, y + 1) != WALL)
						{
							SDL_RenderDrawLine(render,
									lx + ldw * (at(map, x - 1, y) != WALL),
									ly + lh - ldh,
									lx + lw - ldw * (at(map, x + 1, y) != WALL),
									ly + lh - ldh);
						}
						else
							c *= 5;

						if(at(map, x, y - 1) != WALL)
						{
							SDL_RenderDrawLine(render,
									lx + ldw * (at(map, x - 1, y) != WALL),
									ly + ldh,
									lx + lw - ldw * (at(map, x + 1, y) != WALL),
									ly + ldh);
							
						}
						else
							c *= 7;

						if(c % 21 == 0) SDL_RenderDrawLine(render, lx + ldw, ly, lx, ly + ldh);
						if(c % 14 == 0) SDL_RenderDrawLine(render, lx + lw - ldw, ly, lx + lw, ly + ldh);
						if(c % 10 == 0) SDL_RenderDrawLine(render, lx + lw - ldw, ly + lh, lx + lw, ly + lh - ldh);
						if(c % 15 == 0) SDL_RenderDrawLine(render, lx, ly + lh - ldh, lx + ldw, ly + lh);
					}
					break;

				case DOOR:
					SDL_SetRenderDrawColor(render, 0xff, 0xee, 0xee, 0);
					SDL_RenderDrawLine(render, lx, ly + ldh, lx + lw, ly + ldh);
					break;

				case NORMAL_FOOD:
					{
						SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xae, 0);
						SDL_Rect food = {lx + 5 * lw/12, ly + 5 * lh/12, lw/6, lh/6};
						SDL_RenderFillRect(render, &food);
					}
					break;

				case BIG_FOOD:
					{
						SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xae, 0);
						SDL_Rect food = {lx + lw/3, ly + lh/3, lw/3, lh/3};
						SDL_RenderFillRect(render, &food);
					}
					break;
			}
		}
	}
}

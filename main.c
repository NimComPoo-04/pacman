#include <SDL2/SDL.h>
#include <stdlib.h>

#include "map.h"
#include "pacman.h"
#include "ghost.h"

#define WIDTH  (280 * 2)
#define HEIGHT (310 * 2)

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event events = {0};

int main(void)
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
		return -1;

	window = SDL_CreateWindow("PACMAN",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WIDTH+1, HEIGHT+1, SDL_WINDOW_BORDERLESS);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	static map_t map;
	static pacman_t pacman;
	static ghost_t blinky;

	// Don't Need these on the stack
	map = map_create_default(WIDTH, HEIGHT);
	pacman = (pacman_t){
		map.cell_width * 27 / 2, map.cell_height * 23,
		map.cell_width - 1, map.cell_height - 1,
		0, 0, 0};

	blinky = (ghost_t){
		.type = BLINKY,
		.x = map.cell_width * 14,
		.y = map.cell_height * 23,
		.target_mx = pacman.x / map.cell_width,
		.target_my = pacman.y / map.cell_height,
		.visited_nodes = 0,
		.w = map.cell_width - 1,
		.h = map.cell_height - 1,
		.map = &map,
		.stack = {0},
		.top = 0,
		.vx = 1,
		.vy = 1
	};

	int game_loop = 1;
	int move = 0;

	Uint32 time = SDL_GetTicks();
	Uint32 Frame = 0;
	Uint32 Avg = 0;

	while(game_loop)
	{
		while(SDL_PollEvent(&events))
		{
			switch(events.type)
			{
				case SDL_QUIT:
					game_loop = 0;
					break;

				case SDL_KEYDOWN:
					if(events.key.keysym.sym == SDLK_DOWN)
						move = (DOWN);

					else if(events.key.keysym.sym == SDLK_UP)
						move = (UP);

					else if(events.key.keysym.sym == SDLK_LEFT)
						move = (LEFT);

					else if(events.key.keysym.sym == SDLK_RIGHT)
						move = (RIGHT);

					else if(events.key.keysym.sym == SDLK_ESCAPE)
						game_loop = 0;
break;
			}
		}

		int old_move = pacman.direction;
		pacman_change_dir(&pacman, move, 1);
		if(!pacman_move(&pacman, &map))
		{
			pacman_change_dir(&pacman, old_move, 1);
			pacman_move(&pacman, &map);
		}

		ghost_update(&blinky, &pacman, Frame);

		pacman_eat(&pacman, &map);

		if(pacman.score >= map.food_count)
			game_loop = 0;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		map_draw(&map, renderer);
		pacman_draw(&pacman, renderer, (int)SDL_GetTicks());
		ghost_draw(&blinky, renderer);

		SDL_Rect r = {blinky.x, blinky.y, map.cell_width, map.cell_height};
		for(int i = blinky.top - 1; i >= 0; i--)
		{
			SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0);
			SDL_RenderDrawRect(renderer, &r);

			r.y += map.cell_height * (-1 * (blinky.stack[i] == UP) + (blinky.stack[i] == DOWN));
			r.x += map.cell_width * (-1 * (blinky.stack[i] == LEFT) + (blinky.stack[i] == RIGHT));
		}

		if(map_pos_at(&map, pacman.x / map.cell_width, pacman.y / map.cell_height) == WALL)
		{
			SDL_SetRenderDrawColor(renderer, 0xf0, 0, 0xf0, 0);

			SDL_Rect r = {pacman.x / map.cell_width * map.cell_width,
				pacman.y / map.cell_height * map.cell_height,
				pacman.w, pacman.h};

			SDL_RenderDrawRect(renderer, &r);
		}

		Avg += SDL_GetTicks() - time;
//		SDL_Log("%u", );
		while(SDL_GetTicks() - time < 16);
		time = SDL_GetTicks();
		Frame += 1;

		SDL_RenderPresent(renderer);
	}

	SDL_Log("Score: %d\n Time Elapsed on Avarage Per Frame: %d", pacman.score, Avg / Frame);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

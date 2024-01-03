#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>

#include "map.h"
#include "pacman.h"
#include "ghost.h"

#define WIDTH  (280 * 3)
#define HEIGHT (310 * 3)

// Time Taken to maze display

static SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
static SDL_Event events = {0};

static map_t map;
static pacman_t pacman;

static ghost_t blinky, pinky, inky, clyde;

static void init();

static void normal_render(Uint32);
static void party_render(Uint32 Frames);
static void deady_render(Uint32 Frames);

int main(void)
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
		return -1;

	window = SDL_CreateWindow("PACMAN",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WIDTH+1, HEIGHT+1, SDL_WINDOW_BORDERLESS);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Init
	int game_loop = 1;

	Uint32 time = SDL_GetTicks();
	Uint32 Frame = 0;
	Uint32 Avg = 0;

	init();

	int move = 0;
	int party_frames = 0;
	int deady_frames = 0;

	while(game_loop)
	{
		while(SDL_PollEvent(&events))
		{
			switch(events.type)
			{
				case SDL_QUIT: game_loop = 0; break;
				case SDL_KEYDOWN:
					switch(events.key.keysym.sym)
					{
						case SDLK_LEFT: move = LEFT; break;
						case SDLK_RIGHT: move = RIGHT; break;
						case SDLK_UP: move = UP; break;
						case SDLK_DOWN: move = DOWN; break;
						case SDLK_ESCAPE: game_loop = 0; break;
						case SDLK_SPACE:
							SDL_Log("Score: %d == %d",
									pacman.score,
									map.food_count);
							break;
					}
					break;
			}
		}

		// Pacman Update
		int old_move = pacman.entity.direction;
		pacman.entity.direction = move;
		if(!entity_move(&pacman.entity, &map))
		{
			pacman.entity.direction = old_move;
			entity_move(&pacman.entity, &map);
		}

		if(!deady_frames && pacman_eat(&pacman, &map))
		{
#define AZ(az) if(az.state == CHASE || az.state == SCATTER)\
			{\
				az.state = FRIGHTENTED;\
				az.frighten_timer = 3000 / 16;\
			}

			AZ(blinky);
			AZ(inky);
			AZ(pinky);
			AZ(clyde);
		}

		if(pacman.score == map.food_count)
		{
			party_frames = 100;
			map.food_count = 0;
		}
		if(party_frames == 1)
		{
			SDL_Log("Time Elapsed on Stage: %d", Frame * 16 / 1000);
			SDL_Log("Time Elapsed on Avarage Per Frame: %d", Avg / Frame);
			Frame = 0;
			Avg = 0;
			init();
		}

		int isweeded = 0;
		if(!party_frames && !deady_frames)
		{
			// Ghost Update
			isweeded += ghost_update(&blinky, &map, Frame, NULL);
			isweeded += ghost_update(&pinky, &map, Frame, NULL);
			isweeded += ghost_update(&inky, &map, Frame, &blinky);
			isweeded += ghost_update(&clyde, &map, Frame, NULL);
		}

		if(isweeded)
		{
			deady_frames = 100;
		}
		if(deady_frames == 1)
		{
			SDL_Log("Time Elapsed on Stage: %d", Frame * 16 / 1000);
			SDL_Log("Time Elapsed on Avarage Per Frame: %d", Avg / Frame);
			Frame = 0;
			Avg = 0;
			init();
		}

		// RENDERING
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// ghost_update(&blinky, &map, Frame);

		if(party_frames)
		{
			party_render(Frame);
			party_frames--;
		}
		else if(deady_frames)
		{
			deady_render(Frame);
			deady_frames--;
		}
		else
			normal_render(Frame);

		SDL_RenderPresent(renderer);

		// Time Keeping
		Avg += SDL_GetTicks() - time;
		while(SDL_GetTicks() - time < 30); // 60 fps <=> 16.66 spf
		time = SDL_GetTicks();
		Frame += 1;
	}

	free(map.buffer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static void normal_render(Uint32 Frame)
{
	map_draw(&map, renderer, 0xff);
	pacman_draw(&pacman, renderer, Frame);

	ghost_draw(&blinky, renderer, 0xff0000);
	ghost_draw(&pinky, renderer, 0xff55ff);
	ghost_draw(&inky, renderer, 0xD68802);
	ghost_draw(&clyde, renderer, 0xffff);
}

static void party_render(Uint32 Frame)
{
	const int r = (sin(Frame) + 1) / 2 * 255;
	const int g = 255;
	const int b = r;

	map_draw(&map, renderer, (r << 16) | (g << 8) | b);
	pacman_draw(&pacman, renderer, Frame);
}

static void deady_render(Uint32 Frames)
{
	const int r = 255;
	const int g = (sin(Frames) + 1) / 2 * 255;
	const int b = g;

	map_draw(&map, renderer, (r << 16) | (g << 8) | b);
	pacman_draw(&pacman, renderer, Frames);

	ghost_draw(&blinky, renderer, 0xff0000);
	ghost_draw(&pinky, renderer, 0xff55ff);
	ghost_draw(&inky, renderer, 0xD68802);
	ghost_draw(&clyde, renderer, 0xffff);
}

static void init()
{
	map = map_create_default(WIDTH, HEIGHT);
	pacman = pacman_create_default(map.cell_width, map.cell_height);

	blinky = ghost_create(&pacman, &map, BLINKY);
	pinky = ghost_create(&pacman, &map, PINKY);
	inky = ghost_create(&pacman, &map, INKY);
	clyde = ghost_create(&pacman, &map, CLYDE);
}

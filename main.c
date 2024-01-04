#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include "map.h"
#include "pacman.h"
#include "ghost.h"

#define WIDTH  (280 * 2)
#define HEIGHT (310 * 2)

// Time Taken to maze display

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Event events = {0};

static map_t map;
static pacman_t pacman;

static ghost_t blinky, pinky, inky, clyde;

static void init();

static void normal_render(Uint32);
static void party_render(Uint32 Frames);
static void deady_render(Uint32 Frames);


// Exported variables
int wait_time = 7;
int lives = 3;

static int move = 0;
static int party_frames = 0;
static int deady_frames = 0;

static int game_loop = 1;
static int pause = 0;

static Uint32 time = 0;
static Uint32 Frame = 0;
static Uint32 Avg = 0;

void main_loop(void)
{
	if(!game_loop)
	{
		emscripten_cancel_main_loop();
		return;
	}

	time = SDL_GetTicks();

	if(SDL_PollEvent(&events))
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
								 if(!pause)
									 SDL_SetRelativeMouseMode(SDL_FALSE);
								 else
									 SDL_SetRelativeMouseMode(SDL_TRUE);

								 pause = !pause;
								 break;
				       }
				       break;

			case SDL_MOUSEBUTTONDOWN:
				       if(events.button.button == SDL_BUTTON_LEFT)
				       {
					       if(events.button.x > WIDTH - 30 && events.button.y < 30 && pause)
						       game_loop = 0;

					       if(!pause)
						       SDL_SetRelativeMouseMode(SDL_FALSE);
					       else
						       SDL_SetRelativeMouseMode(SDL_TRUE);

					       pause = !pause;
				       }
				       break;

			case SDL_MOUSEMOTION:
				       {
					       const int x = abs(events.motion.xrel);
					       const int y = abs(events.motion.yrel);

					       if(x > y)
					       {
						       if(events.motion.xrel < 0)
							       move = LEFT;
						       else
							       move = RIGHT;
					       }
					       else
					       {
						       if(events.motion.yrel < 0)
							       move = UP;
						       else
							       move = DOWN;
					       }
				       }
				       break;

			case SDL_FINGERMOTION:
				       {
					       const float x = fabs(events.tfinger.dx);
					       const float y = fabs(events.tfinger.dy);

					       if(x > y)
					       {
						       if(events.tfinger.dx < 0)
							       move = LEFT;
						       else
							       move = RIGHT;
					       }
					       else
					       {
						       if(events.tfinger.dy < 0)
							       move = UP;
						       else
							       move = DOWN;
					       }
				       }
				       break;
		}
	}

	if(pause)
		goto RENDER;

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
		if(wait_time != 0)
			wait_time--;
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
		if(lives)
			lives--;
		else
		{
			lives = 3;
			wait_time = 7;
		}
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
RENDER:
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

	if(pause)
	{
		SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0);
		SDL_Rect r = {.x = 10, .y = 10, .w = 10, .h = 20};
		SDL_RenderFillRect(renderer, &r);
		r.x += 15;
		SDL_RenderFillRect(renderer, &r);

		SDL_RenderDrawLine(renderer, WIDTH - 10, 10, WIDTH - 30, 30);
		SDL_RenderDrawLine(renderer, WIDTH - 9, 10, WIDTH - 29, 30);
		SDL_RenderDrawLine(renderer, WIDTH - 8, 10, WIDTH - 28, 30);

		SDL_RenderDrawLine(renderer, WIDTH - 30, 10, WIDTH - 10, 30);
		SDL_RenderDrawLine(renderer, WIDTH - 29, 10, WIDTH - 9, 30);
		SDL_RenderDrawLine(renderer, WIDTH - 28, 10, WIDTH - 8, 30);
	}

	SDL_RenderPresent(renderer);

	// Time Keeping
	Avg += SDL_GetTicks() - time;
	// while(SDL_GetTicks() - time < 16); // 60 fps <=> 16.66 spf NO NEED
	time = SDL_GetTicks();
	Frame += 1;
}

int main(void)
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
		return -1;

	window = SDL_CreateWindow("PACMAN",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WIDTH+1, HEIGHT+1, SDL_WINDOW_BORDERLESS);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Init

	init();
	
	// main_loop;
	emscripten_set_main_loop(main_loop, -1, 1);

	free(map.buffer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

static void draw_lives()
{
	SDL_Rect r = {.x = 12.5 * map.cell_width,
		.y = 15.5 * map.cell_width,
		.w = map.cell_width,
		.h = map.cell_height};

	float tx = 0.5;

	for(int i = 0; i < lives; i++)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderFillRect(renderer, &r);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0, 0);
		for(int x = -r.w / 2; x <= r.w / 2; x++)
		{
			for(int y = -r.h / 2; y <= r.h / 2; y++)
			{
				if((y < (x + 2) * tx && y > - (x + 2) * tx))
					continue;
				if(x * x + y * y <= r.h * r.h / 4)
					SDL_RenderDrawPoint(renderer, x + r.x + r.w/2, y + r.y + r.h/2);

			}
		}
		r.x += r.w;
	}
}

static void normal_render(Uint32 Frame)
{
	map_draw(&map, renderer, (((7 - wait_time) * 51) << 16) | (((7 - wait_time) * 51) << 8) | 0xff);
	pacman_draw(&pacman, renderer, Frame);

	ghost_draw(&blinky, renderer, 0xff0000);
	ghost_draw(&pinky, renderer, 0xff55ff);
	ghost_draw(&inky, renderer, 0xD68802);
	ghost_draw(&clyde, renderer, 0xffff);

	draw_lives();
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

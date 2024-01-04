#include <stdlib.h>
#include "ghost.h"

static void path_finding(ghost_t *g, map_t *m)
{
	int free_spaces[4] = {0};

	int c = 0;

	for(int i = 0; i < 4; i++)
	{
		entity_t e = g->entity;

		e.direction = i;
		if(entity_move(&e, m))
		{
			free_spaces[i] = 1;
			c++;
		}
	}

	switch(g->entity.direction)
	{
		case 0: free_spaces[1] = 0; break;
		case 1: free_spaces[0] = 0; break;
		case 2: free_spaces[3] = 0; break;
		case 3: free_spaces[2] = 0; break;
	}

	/*
	SDL_Log("%d %d : %d %d : {%d %d %d %d}",
			g->entity.x, g->entity.y,
			g->entity.x / m->cell_width, g->entity.y / m->cell_height,
			free_spaces[0], free_spaces[1], free_spaces[2], free_spaces[3]);
			*/

#define DIST(a, b) hypot(g->entity.x / m->cell_width + a - g->target_x,\
		g->entity.y / m->cell_height + b - g->target_y)

	int min_dist = 999999;
	int dir = -1;
	if(free_spaces[0] && min_dist > DIST(0, -1))
	{
		min_dist = DIST(0, -1);
		dir = UP;
	}
	if(free_spaces[1] && min_dist > DIST(0, 1))
	{
		min_dist = DIST(0, 1);
		dir = DOWN;
	}
	if(free_spaces[2] && min_dist > DIST(-1, 0))
	{
		min_dist = DIST(-1, 0);
		dir = LEFT;
	}
	if(free_spaces[3] && min_dist > DIST(1, 0))
	{
		min_dist = DIST(1, 0);
		dir = RIGHT;
	}

	g->entity.direction = dir;
	entity_move(&g->entity, m);	

#undef DIST
}

int ghost_update(ghost_t *g, map_t *m, Uint32 frames, void *v)
{
	extern int wait_time;
	if((int)frames < wait_time * 1000/16)
		return 0;

	switch(g->state)
	{
		case FRIGHTENTED:
			if(g->frighten_timer == 3000 / 16)
			{
				switch(g->entity.direction)
				{
					case 0: g->entity.direction = 1; break;
					case 1: g->entity.direction = 0; break;
					case 2: g->entity.direction = 3; break;
					case 3: g->entity.direction = 2; break;
				}	
			}

			for(int i = 0; i < 10; i++)
			{
				g->entity.direction = rand()%4;
				if(entity_move(&g->entity, m))
					break;
			}

			if(g->frighten_timer == 0)
			{
				if(g->chase_timer)
				{
					g->state = CHASE;
				}
				else if(g->scatter_timer)
				{
					g->state = SCATTER;
				}
				else
				{
					g->chase_timer = 250;
					g->state = SCATTER;
				}
			}

			g->frighten_timer--;

			if(entity_collision_entity(&g->pacman->entity, &g->entity))
				g->state = DEAD;

			return 0;

		case DEAD:
			if(g->entity.x / m->cell_width != 14
					|| g->entity.y / m->cell_height != 14)
			{
				g->target_x = 14;
				g->target_y = 14;
				g->entity.allow_door = 1;
			}
			else
			{
				g->frighten_timer = 0;
				g->entity.allow_door = 0;
				g->state = START;
			}

			path_finding(g, m);

			return 0;

		case START:
			g->target_x = 10;
			g->target_y = 10;

			if(g->entity.x/m->cell_width == g->target_x
					&& g->entity.x/m->cell_width == g->target_y)
			{
				g->chase_timer = 250;
				g->state = CHASE;
				g->entity.allow_door = 0;
			}

			path_finding(g, m);
			return entity_collision_entity(&g->pacman->entity, &g->entity);

		case SCATTER:
			g->scatter_timer--;
			if(g->scatter_timer == 0)
			{
				g->chase_timer = 250;
				g->state = CHASE;
			}
			g->target_reassign(g, m, v);	
			path_finding(g, m);
			return entity_collision_entity(&g->pacman->entity, &g->entity);

		case CHASE:
			g->chase_timer--;
			if(g->chase_timer == 0)
			{
				g->scatter_timer = 250;
				g->state = SCATTER;
			}
			g->target_reassign(g, m, v);	
			path_finding(g, m);
			return entity_collision_entity(&g->pacman->entity, &g->entity);
	}

	return 0;

}

void ghost_debug_draw(ghost_t *g, SDL_Renderer *ren)
{
	if(g->state == CHASE)
		entity_debug_draw(&g->entity, ren, 0xff0000);
	else if(g->state == FRIGHTENTED)
		entity_debug_draw(&g->entity, ren, 0xffff00);
	else
		entity_debug_draw(&g->entity, ren, 0x00ffff);
}

void blinky_target(ghost_t *g, map_t *m, void *v)
{
	(void)v;
	switch(g->state)
	{
		case CHASE:
			g->target_x = g->pacman->entity.x / m->cell_width;
			g->target_y = g->pacman->entity.y / m->cell_height;
			break;

		case SCATTER:
			g->target_x = 26;
			g->target_y = -2;
			break;
	}
}

void pinky_target(ghost_t *g, map_t *m, void *v)
{
	(void)v;
	switch(g->state)
	{
		case CHASE:
			g->target_x = g->pacman->entity.x / m->cell_width;
			g->target_y = g->pacman->entity.y / m->cell_height;

			switch(g->pacman->entity.direction)
			{
				case UP:    g->target_y -= 2; break;
				case DOWN:  g->target_y += 2; break;
				case LEFT:  g->target_x -= 2; break;
				case RIGHT: g->target_x += 2; break;
			}

			break;

		case SCATTER:
			g->target_x = 1;
			g->target_y = -2;
			break;
	}
}
void inky_target(ghost_t *g, map_t *m, void *v)
{
	(void)v;
	ghost_t *blinky = (ghost_t *)v;

	switch(g->state)
	{
		case CHASE:
			g->target_x = g->pacman->entity.x / m->cell_width;
			g->target_y = g->pacman->entity.y / m->cell_height;

			switch(g->pacman->entity.direction)
			{
				case UP:    g->target_y -= 2; break;
				case DOWN:  g->target_y += 2; break;
				case LEFT:  g->target_x -= 2; break;
				case RIGHT: g->target_x += 2; break;
			}

			g->target_y = (g->target_y - blinky->entity.y / m->cell_width) * 2;
			g->target_x = (g->target_x - blinky->entity.x / m->cell_height) * 2;

			// SDL_Log("%d %d", g->target_x, g->target_y);

			break;

		case SCATTER:
			g->target_x = 26;
			g->target_y = 31;
			break;
	}
}

void clyde_target(ghost_t *g, map_t *m, void *v)
{
	(void)v;
	switch(g->state)
	{
		case CHASE:
			g->target_x = g->pacman->entity.x / m->cell_width;
			g->target_y = g->pacman->entity.y / m->cell_height;

			int dist = hypot(g->pacman->entity.x - g->entity.x,
					g->pacman->entity.y - g->entity.y) / m->cell_width;

			if(dist < 8)
			{
				g->target_x = 1;
				g->target_y = 31;
			}

			break;

		case SCATTER:
			g->target_x = 1;
			g->target_y = 31;
			break;

	}
}

ghost_t ghost_create(pacman_t *p, map_t *m, int type)
{
	ghost_t g = {
		.entity = {
			.direction = -1,
			.vx = m->cell_width - 1,
			.vy = m->cell_height - 1,
			.w = m->cell_width - 1,
			.h = m->cell_height - 1,
		},
		.pacman = p,
		.state = START
	};
	switch(type)
	{
		case BLINKY:
			g.entity.x = m->cell_width * 14.5;
			g.entity.y = m->cell_height * 14.5;
			g.target_reassign = blinky_target;
			break;

		case PINKY:
			g.entity.x = m->cell_width * 13.5;
			g.entity.y = m->cell_height * 14.5;
			g.target_reassign = pinky_target;
			break;

		case CLYDE:
			g.entity.x = m->cell_width * 13.5;
			g.entity.y = m->cell_height * 13.5;
			g.target_reassign = clyde_target;
			break;

		case INKY:
			g.entity.x = m->cell_width * 14.5;
			g.entity.y = m->cell_height * 13.5;
			g.target_reassign = inky_target;
			break;
	}

	return g;
}

void ghost_draw(ghost_t *g, SDL_Renderer *ren, Uint32 color)
{
	if(g->state == FRIGHTENTED)
	{
		if(g->frighten_timer < 1000/16)
			SDL_SetRenderDrawColor(ren, SDL_GetTicks() % 255, SDL_GetTicks() % 255, 0xff, 0);
		else
			SDL_SetRenderDrawColor(ren, 0, 0, 0xff, 0);
	}
	else
		SDL_SetRenderDrawColor(ren, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, 0);

	float cw =  g->entity.w * 4 / 5;
	float ch =  g->entity.h * 4 / 5;

	float offset_x = g->entity.x + cw / 4;
	float offset_y = g->entity.y + ch;

	if(g->state != DEAD)
	{
		for(int x = -cw; x <= cw; x++)
		{
			for(int y = -ch; y <= ch; y++)
			{
				float tx = x / cw;

				if(tx < -0.86 || tx > 0.86)
					continue;

				float cty = -3 * pow(fabs(tx), 2.8) + 2;
				float dty = 0.2 * (cos(10 * tx + SDL_GetTicks()/100) + 1);

				SDL_RenderDrawLine(ren, x + offset_x,
						-cty * cw + offset_y,
						x + offset_x,
						-dty * cw + offset_y);
			}
		}
	}
	else
	{
		SDL_Rect r = {.x = g->entity.x - g->entity.w / 3,
			.y = g->entity.y +  - g->entity.h / 2,
			.w = g->entity.w, g->entity.h * 3 / 4};

		SDL_RenderDrawRect(ren, &r);
	}

	SDL_Rect r1 = {.x = g->entity.x - g->entity.w / 4,
		.y = g->entity.y - g->entity.h / 4,
		.w = g->entity.w/3, .h = g->entity.h/3};

	SDL_Rect r2 = {.x = g->entity.x + g->entity.w / 4,
		.y = g->entity.y - g->entity.h / 4,
		.w = g->entity.w/3, .h = g->entity.w/3};

	if(g->state == FRIGHTENTED && g->frighten_timer < 1000/16)
	{
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
	}
	else
		SDL_SetRenderDrawColor(ren, 0xff, 0xff, 0xff, 0xff);

	SDL_RenderFillRect(ren, &r1);
	SDL_RenderFillRect(ren, &r2);

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
	SDL_RenderDrawRect(ren, &r1);
	SDL_RenderDrawRect(ren, &r2);
}

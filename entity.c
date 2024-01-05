#include "entity.h"

int entity_move(entity_t *e, map_t *m)
{
	int tx = e->x;
	int ty = e->y;

	int vx = 0;
	int vy = 0;

	switch(e->direction)
	{
		case UP:
			e->y -= e->vy;
			vy = -1;
			break;

		case DOWN:
			e->y += e->vy;
			vy = 1;
			break;

		case LEFT:
			e->x -= e->vx;
			vx = -1;
			break;

		case RIGHT:
			e->x += e->vx;
			vx = 1;
			break;
	}
	if(entity_collision_map(e, m, DOOR) && e->direction == DOWN && !e->allow_door)
	{
		e->x = tx;
		e->y = ty;

		// FIXME: EVIL LOOP REMOVE
		// This loop is not good for health at all
		// Probably fine for slow vx and vy.
		while(!entity_collision_map(e, m, DOOR))
		{
			e->x += vx;
			e->y += vy;
		}

		e->x -= vx;
		e->y -= vy;

		return 0;
	}
	if(entity_collision_map(e, m, WALL))	
	{
		e->x = tx;
		e->y = ty;

		// FIXME: EVIL LOOP REMOVE
		// This loop is not good for health at all
		// Probably fine for slow vx and vy.
		while(!entity_collision_map(e, m, WALL))
		{
			e->x += vx;
			e->y += vy;
		}

		e->x -= vx;
		e->y -= vy;

		return 0;
	}

	if(e->x - e->w / 2 > m->width * m->cell_width)
		e->x = m->cell_width / 2;

	if(e->x + e->w < 0)
		e->x = m->width * m->cell_width - m->cell_width/2;

	return 1;
}

int entity_collision_entity(entity_t *e1, entity_t *e2)
{
	const int d = hypot(e1->x - e2->x, e1->y - e2->y);
	return d < e1->w/2 || d < e2->w/2;
}

int entity_collision_map(entity_t *e, map_t *m, int type)
{
	float x = 1.0 * e->x / m->cell_width;
	float y = 1.0 * e->y / m->cell_height;

	float w = 1.0 * e->w / m->cell_width / 2;
	float h = 1.0 * e->h / m->cell_height / 2;

	if(map_pos_at(m, x + w, y) == type) return 1;
	if(map_pos_at(m, x - w, y) == type) return 1;
	if(map_pos_at(m, x, y + h) == type) return 1;
	if(map_pos_at(m, x, y - h) == type) return 1;

	if(map_pos_at(m, x - w, y - h) == type) return 1;
	if(map_pos_at(m, x + w, y - h) == type) return 1;
	if(map_pos_at(m, x - w, y + h) == type) return 1;
	if(map_pos_at(m, x + w, y + h) == type) return 1;

	return 0;
}

void entity_debug_draw(entity_t *e, SDL_Renderer *re, uint32_t col)
{
	SDL_Rect r = {e->x - e->w/2, e->y - e->h/2, e->w, e->h};
	SDL_SetRenderDrawColor(re, (col >> 16) & 0xff, (col >> 8) & 0xff, col & 0xff, 0);
	SDL_RenderDrawRect(re, &r);
}

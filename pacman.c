#include "pacman.h"

void pacman_change_dir(pacman_t *p, int d, int v)
{
	p->direction = d;

	switch(d)
	{
		case UP:    p->vx = 0; p->vy = -v; break;
		case DOWN:  p->vx = 0; p->vy = v; break;
		case LEFT:  p->vx = -v; p->vy = 0; break;
		case RIGHT: p->vx = v; p->vy = 0; break;
	}
}

int pacman_move(pacman_t *p, map_t *m)
{
	int tx= p->x;
	int ty= p->y;

	p->x = p->x + p->vx;
	p->y = p->y + p->vy;

	if(p->x > m->cell_width * m->width) p->x = 0;
	if(p->x < 0) p->x = m->cell_width * (m->width - 1);
	if(p->y > m->cell_height * m->height) p->y = 0;
	if(p->y < 0) p->y = m->cell_height * (m->height - 1);

	char k = map_pos_at(m, p->x / m->cell_width, p->y / m->cell_height);
	if(k == WALL)
	{
		p->x = tx;
		p->y = ty;
		return 0;
	}

	k = map_pos_at(m, (p->x + p->w) / m->cell_width, p->y / m->cell_height);
	if(k == WALL)
	{
		p->x = tx;
		p->y = ty;
		return 0;
	}

	k = map_pos_at(m, p->x / m->cell_width, (p->y + p->h) / m->cell_height);
	if(k == WALL)
	{
		p->x = tx;
		p->y = ty;
		return 0;
	}

	k = map_pos_at(m, (p->x + p->w) / m->cell_width, (p->y + p->h) / m->cell_height);
	if(k == WALL)
	{
		p->x = tx;
		p->y = ty;
		return 0;
	}

	return 1;
}

void pacman_draw(pacman_t *p, SDL_Renderer *r, int time)
{
	SDL_SetRenderDrawColor(r, 0xff, 0xff, 0, 0);
	SDL_Rect rec = {p->x, p->y, p->w, p->h};
	SDL_RenderFillRect(r, &rec);
}

void pacman_eat(pacman_t *p, map_t *m)
{
	int i = (p->x + p->w / 2) / m->cell_width;
	int j = (p->y + p->h / 2) / m->cell_height;

	if(map_pos_at(m, i, j) == NORMAL_FOOD || map_pos_at(m, i, j) == BIG_FOOD)
	{
		m->buffer[j * m->width + i] = NONE;
		p->score++;
	}
}

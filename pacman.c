#include "pacman.h"

#define PI 3.1415

float RAD(int a)
{
	return PI * a / 180;
}

void pacman_draw(pacman_t *p, SDL_Renderer *r, int time)
{
	SDL_SetRenderDrawColor(r, 0xff, 0xff, 0, 0);

	for(int x = -p->entity.w * 4/5; x <= p->entity.w * 4/5; x++)
	{
		for(int y = -p->entity.h * 4/5; y <= p->entity.h * 4/5; y++)
		{
			if(x * x + y * y <= p->entity.h * p->entity.h * 16 / 25)
				SDL_RenderDrawPoint(r, x + p->entity.x, y + p->entity.y);

		}
	}

	float tx = tan(RAD(time % 55));

	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);

	for(int x = -p->entity.w * 4/5; x <= p->entity.w * 4/5; x++)
	{
		for(int y = -p->entity.h * 4/5; y <= p->entity.h * 4/5; y++)
		{
			switch(p->entity.direction)
			{
				case RIGHT:
					if((y < (x + 5) * tx && y > - (x + 5) * tx ))
						SDL_RenderDrawPoint(r, x + p->entity.x, y + p->entity.y);
					break;

				case LEFT:
					if((y < (-x + 5) * tx && y > - (-x + 5) * tx ))
						SDL_RenderDrawPoint(r, x + p->entity.x, y + p->entity.y);
					break;

				case DOWN:
					if((x < (y + 5) * tx && x > - (y + 5) * tx ))
						SDL_RenderDrawPoint(r, x + p->entity.x, y + p->entity.y);
					break;

				case UP:
					if((x < (-y + 5) * tx && x > -(-y + 5) * tx ))
						SDL_RenderDrawPoint(r, x + p->entity.x, y + p->entity.y);
					break;
			}
		}
	}
}

int pacman_eat(pacman_t *p, map_t *m)
{
	int x = p->entity.x / m->cell_width;
	int y = p->entity.y / m->cell_height;

	int k = map_pos_at(m, x, y);

	if( k == BIG_FOOD || k ==  NORMAL_FOOD)
	{
		m->buffer[(p->entity.x / m->cell_width) + (p->entity.y / m->cell_height) * m->width] = NONE;
		p->score++;
	}

	return k == BIG_FOOD;
}

pacman_t pacman_create_default(int cw, int ch)
{
	return (pacman_t){
		.entity = {
		.direction = -1,
			.x = cw * 14.5,
			.y = ch * 23.5,
			.w = cw - 1,
			.h = ch - 1,
			.vx = cw / 5,
			.vy = ch / 5
		},
			.score = 0
	};
}

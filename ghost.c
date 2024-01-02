#include "ghost.h"

void ghost_update_target(ghost_t *g, int x, int y)
{
	g->target_mx = x;
	g->target_my = y;
}

int ghost_add_move(ghost_t *g, char move)
{
	if(g->top >= 1024)
		return 0;

	g->stack[g->top++] = move;
	return 1;
}

int ghost_get_move(ghost_t *g)
{
	if(g->top <= 0)
		return -1;
	return g->stack[--g->top];
}

int ghost_drop_stack(ghost_t *g)
{
	return g->top = 0;
}

#define ABS(a) (a * ((a > 0) - (a < 0)))

static int huristics(int x, int y, int tx, int ty)
{
	return (tx - x) * (tx - x) + (ty - y) * (ty - y);
}

static int F(int x, int y, int tx, int ty, int cost)
{
	return huristics(x, y, tx, ty) + cost;
}

// We are performing sort of a depth first search, this is because
// our graph has a special property the distance between all nodes is one.
// So if we find a node which is better in terms of cost then we can
// follow the node with some level of confidence, knowing down the line
// the answer would not change, Although this does mean we do not get the
// best possible solutions.
//
// This algo uses the huristic and cost function idea like the a* algorithm

// a* type of path finding
static int path_find(ghost_t *g, map_t *m, char *visited_nodes, int x, int y, int tx, int ty, int cost)
{
	// Don't Go Out of Bounds
	if(x >= m->width || x < 0 || y >= m->height || y < 0)
		return 0;

	const int tmp = map_pos_at(m, x, y);
	// If the Current Node is a wall we ain't going nowhere
	if(tmp == WALL)
		return 0;

	// If the Current Node is the position of target, done
	if(x == tx && y == ty)
		return 1;

	// If we have already visited this node don't bother
	if(visited_nodes[y * m->width + x] == 1)
		return 0;

	// Well Now its visited
	visited_nodes[y * m->width + x] = 1;

	int costs[] = {
		F(x + 1, y, tx, ty, cost + 1),
		F(x - 1, y, tx, ty, cost + 1),
		F(x, y + 1, tx, ty, cost + 1),
		F(x, y - 1, tx, ty, cost + 1),
	};

	int ids[] = { RIGHT, LEFT, DOWN, UP };

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(costs[j + 1] < costs[j])
			{
				int tmp = costs[j + 1];
				costs[j + 1] = costs[j];
				costs[j] = tmp;

				tmp = ids[j + 1];
				ids[j + 1] = ids[j];
				ids[j] = tmp;
			}
		}
	}

	for(int i = 0; i < 4; i++)
	{
		switch(ids[i])
		{
			case RIGHT:
				if(path_find(g, m, visited_nodes, x + 1, y, tx, ty, cost + 1))
				{
					ghost_add_move(g, RIGHT);
					return 1;
				}
				break;

			case LEFT:
				if(path_find(g, m, visited_nodes, x - 1, y, tx, ty, cost + 1))
				{
					ghost_add_move(g, LEFT);
					return 1;
				}
				break;

			case DOWN:
				if(path_find(g, m, visited_nodes, x, y + 1, tx, ty, cost + 1))
				{
					ghost_add_move(g, DOWN);
					return 1;
				}
				break;

			case UP:
				if(path_find(g, m, visited_nodes, x, y - 1, tx, ty, cost + 1))
				{
					ghost_add_move(g, UP);
					return 1;
				}
				break;
		}
	}

	return 0;
}

void ghost_generate_path(ghost_t *g)
{
	if(g->visited_nodes == NULL)
		g->visited_nodes = calloc(sizeof(char), g->map->width * g->map->height);
	else
	{
		ghost_drop_stack(g);
	}

	path_find(g, g->map,
		  g->visited_nodes,
		  (g->x + g->w/2) / g->map->cell_width,
		  (g->y + g->h/2) / g->map->cell_height,
		  g->target_mx,
		  g->target_my, 0);

	g->from_mx = (g->x + g->w/2) / g->map->cell_width;
	g->from_my = (g->y + g->h/2) / g->map->cell_height;
}

void ghost_update(ghost_t *g, pacman_t *p, int ticks)
{
	static pacman_t old_p;

	if(ticks % 855)
		g->state = SCATTER;

	if(ticks % 1000)
		g->state = FOLLOW;

	if(g->state == FOLLOW)
	{
		if(ticks % 20 == 0 && (old_p.x != p->x || old_p.y != p->y))
		{
			ghost_update_target(g,
					p->x / g->map->cell_width,
					p->y / g->map->cell_height);
			ghost_generate_path(g);
		}

		old_p = *p;
	}
	else
	{
		ghost_update_target(g, 1, 1);
		ghost_generate_path(g);
	}

	int d = ghost_get_move(g);
	switch(d)
	{
		case UP: g->y -= g->vy; break;
		case DOWN: g->y += g->vy; break;
		case LEFT: g->x -= g->vx; break;
		case RIGHT: g->x += g->vx; break;
	}

	if((g->x + g->w/2) / g->map->cell_width == g->from_mx &&
			(g->y + g->h/2) / g->map->cell_height == g->from_my)
	{
		ghost_add_move(g, d);
	}
	else
	{
		g->from_mx = (g->x + g->w/2) / g->map->cell_width;
		g->from_my = (g->y + g->h/2) / g->map->cell_height;
	}
}

void ghost_draw(ghost_t *g, SDL_Renderer *r)
{
	SDL_SetRenderDrawColor(r, 0xff, 0x00, 0, 0);
	SDL_Rect rec = {g->x, g->y, g->w, g->h};
	SDL_RenderFillRect(r, &rec);
}

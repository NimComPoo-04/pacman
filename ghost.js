const START = 0
const SCATTER = 1
const CHASE = 2
const FRIGHTENTED = 3
const DEAD = 4

class Ghost extends Entity
{
	constructor(x, y, map, target_reassign, time)
	{
		super(x * map.cell_width, y * map.cell_height,
			map.cell_width - 1,
			map.cell_height - 1,
			map.cell_width / 5,
			map.cell_height / 5)

		this.state = START
		this.target_reassign = target_reassign 
		this.state_timer = current_time()
		this.wait_time = 1000 * time

		this.allow_door = false
		this.prev_state = 0

		this.target_x = 0
		this.target_y = 0
	}

	DIST(a, b)
	{
		return Math.hypot(this.x / map.cell_width + a - this.target_x,
			this.y / map.cell_height + b - this.target_y)
	}

	#path_finding(map)
	{
		let free_spaces = [false, false, false, false];

		let c = 0;

		for(let i = 0; i < 4; i++)
		{
			let e = Object.assign({}, this)
			Object.setPrototypeOf(e, Ghost.prototype)
			e.direction = i;

			if(e.move(map))
			{
				free_spaces[i] = true;
				c++;
			}
		}

		switch(this.direction)
		{
			case 0: free_spaces[1] = false; break;
			case 1: free_spaces[0] = false; break;
			case 2: free_spaces[3] = false; break;
			case 3: free_spaces[2] = false; break;
		}

		let min_dist = 999999;
		let dir = -1;

		if(free_spaces[0] && min_dist > this.DIST(0, -1))
		{
			min_dist = this.DIST(0, -1);
			dir = UP;
		}
		if(free_spaces[1] && min_dist > this.DIST(0, 1))
		{
			min_dist = this.DIST(0, 1);
			dir = DOWN;
		}
		if(free_spaces[2] && min_dist > this.DIST(-1, 0))
		{
			min_dist = this.DIST(-1, 0);
			dir = LEFT;
		}
		if(free_spaces[3] && min_dist > this.DIST(1, 0))
		{
			min_dist = this.DIST(1, 0);
			dir = RIGHT;
		}

		this.direction = dir;
		this.move(map);
	}

	update(map, pacman, v = null)
	{
		if(current_time() < this.wait_time)
			return 0;

		switch(this.state)
		{
			case FRIGHTENTED:
				if(current_time() - this.state_timer == 0)
				{
					switch(this.direction)
					{
						case 0: this.direction = 1; break;
						case 1: this.direction = 0; break;
						case 2: this.direction = 3; break;
						case 3: this.direction = 2; break;
					}	
				}

				while(!this.move(map))
					this.direction = parseInt(Math.random() * 4);

				if(current_time() - this.state_timer > 5000)
				{
					this.state = this.prev_state;
					this.state_timer = current_time() - 5000;
				}

				if(this.collision(pacman))
					this.state = DEAD;

				return 0;

			case DEAD:
				if(parseInt(this.x / map.cell_width) != 14
					|| parseInt(this.y / map.cell_height) != 14)
				{
					this.target_x = 14;
					this.target_y = 14;
					this.allow_door = true;
				}
				else
				{
					this.state_timer = 0;
					this.allow_door = false;
					this.state = START;
				}

				this.#path_finding(map);

				return 0;

			case START:
				this.target_x = 11;
				this.target_y = 11;

				if(parseInt(this.x / map.cell_width) == this.target_x
					&& parseInt(this.x / map.cell_width) == this.target_y)
				{
					this.state_timer = current_time();
					this.state = CHASE;
					this.allow_door = 0;
				}

				this.#path_finding(map);
				return this.collision(pacman);

			case SCATTER:
				if(current_time() - this.state_timer >= 10000)
				{
					this.state_timer = current_time();
					this.state = CHASE;
				}

				this.target_reassign(this, pacman, map, v)
				this.#path_finding(map)

				return this.collision(pacman)

			case CHASE:
				if(current_time() - this.state_timer >= 30000)
				{
					this.state_timer = current_time();
					this.state = SCATTER;
				}

				this.target_reassign(this, pacman, map, v);
				this.#path_finding(map);

				return this.collision(pacman);
		}

		return 0;

	}

	debug_draw(ctx, color = undefined)
	{
		if(this.state == CHASE)
			super.draw(ctx, 0xff0000)
		else if(this.state == FRIGHTENTED)
			super.draw(ctx, 0xffff00)
		else
			super.draw(ctx, 0x00ffff);
	}


	draw(ctx, color)
	{
		if(this.state == FRIGHTENTED)
		{
			if(current_time() - this.state_timer > 4000)
			{
				ctx.strokeStyle = ['white', 'blue'][current_time() % 2]
			}
			else
				ctx.strokeStyle = 'blue'
		}
		else
			ctx.strokeStyle = color

		const cw =  this.w * 4 / 5;
		const ch =  this.h * 4 / 5;

		const offset_x = this.x + cw / 4;
		const offset_y = this.y + ch;


		if(this.state != DEAD)
		{
			ctx.beginPath()
			for(let x = -cw * 0.86; x <= cw * 0.86; x++)
			{
				const tx = x / cw;

				const cty = -3 * Math.pow(Math.abs(tx), 2.8) + 2;
				const dty = 0.2 * (Math.cos(10 * tx + current_time()/100) + 1);

				ctx.moveTo(x + offset_x, -cty * cw + offset_y)
				ctx.lineTo(x + offset_x, -dty * cw + offset_y)
			}
			ctx.stroke()
		}
		else
		{
			ctx.strokeRect(this.x - this.w / 3, this.y +  - this.h / 2, this.w, this.h * 3 / 4);
		}

		if(this.state == FRIGHTENTED && current_time() - this.state_timer > 4000)
			ctx.fillStyle = '#ff0000';
		else
			ctx.fillStyle = '#ffffff';

		ctx.fillRect(this.x - this.w / 4, this.y - this.h / 4, this.w/3, this.h/3);
		ctx.fillRect(this.x + this.w / 4, this.y - this.h / 4, this.w/3, this.w/3);

		ctx.strokeStyle = '#000000';

		ctx.strokeRect(this.x - this.w / 4, this.y - this.h / 4, this.w/3, this.h/3);
		ctx.strokeRect(this.x + this.w / 4, this.y - this.h / 4, this.w/3, this.w/3);
	}

	static blinky_target(g, p, m)
	{
		switch(g.state)
		{
			case CHASE:
				g.target_x = parseInt(p.x / m.cell_width);
				g.target_y = parseInt(p.y / m.cell_height);
				break;

			case SCATTER:
				g.target_x = 26;
				g.target_y = -2;
				break;
		}
	}

	static pinky_target(g, p, m)
	{
		switch(g.state)
		{
			case CHASE:
				g.target_x = parseInt(p.x / m.cell_width);
				g.target_y = parseInt(p.y / m.cell_height);

				switch(p.direction)
				{
					case UP:    g.target_y -= 2; break;
					case DOWN:  g.target_y += 2; break;
					case LEFT:  g.target_x -= 2; break;
					case RIGHT: g.target_x += 2; break;
				}

				break;

			case SCATTER:
				g.target_x = 1;
				g.target_y = -2;
				break;
		}
	}

	static inky_target(g, p, m, blinky)
	{
		switch(g.state)
		{
			case CHASE:
				g.target_x = parseInt(p.x / m.cell_width);
				g.target_y = parseInt(p.y / m.cell_height);

				switch(p.direction)
				{
					case UP:    g.target_y -= 2; break;
					case DOWN:  g.target_y += 2; break;
					case LEFT:  g.target_x -= 2; break;
					case RIGHT: g.target_x += 2; break;
				}

				g.target_y = parseInt(g.target_y - blinky.y / m.cell_width) * 2;
				g.target_x = parseInt(g.target_x - blinky.x / m.cell_height) * 2;

				break;

			case SCATTER:
				g.target_x = 26;
				g.target_y = 31;
				break;
		}
	}

	static clyde_target(g, p, m)
	{
		switch(g.state)
		{
			case CHASE:
				g.target_x = parseInt(p.x / m.cell_width);
				g.target_y = parseInt(p.y / m.cell_height);

				const dist = Math.hypot(p.x - g.x,
					p.y - g.y) / m.cell_width;

				if(dist < 8)
				{
					g.target_x = 1;
					g.target_y = 31;
				}

				break;

			case SCATTER:
				g.target_x = 1;
				g.target_y = 31;
				break;

		}
	}
}


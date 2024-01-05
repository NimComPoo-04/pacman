const NO_MOVE = -1
const UP = 0
const DOWN = 1
const LEFT = 2
const RIGHT = 3

class Entity
{
	constructor(x = 0, y = 0, w = 0, h = 0, vx = 0, vy = 0)
	{
		this.direction = NO_MOVE
		this.allow_door = false

		this.x = x
		this.y = y
		this.w = w
		this.h = h

		this.vx = vx
		this.vy = vy
	}

	move(map)
	{
		const tx = this.x
		const ty = this.y

		let vx = 0
		let vy = 0

		switch(this.direction)
		{
			case UP:
				this.y -= this.vy;
				vy = -1;
				break;

			case DOWN:
				this.y += this.vy;
				vy = 1;
				break;

			case LEFT:
				this.x -= this.vx;
				vx = -1;
				break;

			case RIGHT:
				this.x += this.vx;
				vx = 1;
				break;
		}

		if(this.collision(map, DOOR) && this.direction == DOWN && !this.allow_door)
		{
			console.log(this.allow_door)

			this.x = tx;
			this.y = ty;

			// FIXME: EVIL LOOP REMOVE
			// This loop is not good for health at all
			// Probably fine for slow vx and vy.
			while(!this.collision(map, DOOR))
			{
				this.x += vx;
				this.y += vy;
			}

			this.x -= vx;
			this.y -= vy;

			return false;
		}

		if(this.collision(map, WALL))
		{

			this.x = tx;
			this.y = ty;

			// FIXME: EVIL LOOP REMOVE
			// This loop is not good for health at all
			// Probably fine for slow vx and vy.
			while(!this.collision(map, WALL))
			{
				this.x += vx;
				this.y += vy;
			}

			this.x -= vx;
			this.y -= vy;

			return false;
		}

		if(this.x - this.w / 2 > map.width * map.cell_width)
			this.x = map.cell_width / 2;

		if(this.x + this.w < 0)
			this.x = map.width * map.cell_width - map.cell_width/2;

		return true;
	}

	collision(other, type)
	{
		if(other instanceof Entity)
		{
			let d = Math.hypot(this.x - other.x, this.y - other.y);
			return d < this.w/2 || d < other.w/2;
		}
		else if(other instanceof Map)
		{
			let x = this.x / other.cell_width;
			let y = this.y / other.cell_height;

			let w = this.w / other.cell_width / 2;
			let h = this.h / other.cell_height / 2;

			if(other.at(x + w, y) == type) return true;
			if(other.at(x - w, y) == type) return true;
			if(other.at(x, y + h) == type) return true;
			if(other.at(x, y - h) == type) return true;

			if(other.at(x - w, y - h) == type) return true;
			if(other.at(x + w, y - h) == type) return true;
			if(other.at(x - w, y + h) == type) return true;
			if(other.at(x + w, y + h) == type) return true;

			return false;
		}
		else
			throw 'Not Type Specified!!'
	}

	draw(ctx, color)
	{
		ctx.strokeStyle = color
		ctx.strokeRect(this.x - this.w/2, this.y - this.h/2, this.w, this.h)
	}
}

const NORMAL_FOOD = 9
const BIG_FOOD = 8
const NONE = 7
const DOOR = 6
const WALL = 5

const default_map = 
[
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 5, 5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 5, 
5, 9, 5, 5, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 5, 5, 9, 5, 
5, 8, 5, 5, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 5, 5, 8, 5, 
5, 9, 5, 5, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 5, 5, 9, 5, 
5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 5, 
5, 9, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 9, 5, 
5, 9, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 9, 5, 
5, 9, 9, 9, 9, 9, 9, 5, 5, 9, 9, 9, 9, 5, 5, 9, 9, 9, 9, 5, 5, 9, 9, 9, 9, 9, 9, 5, 
5, 5, 5, 5, 5, 5, 9, 5, 5, 5, 5, 5, 7, 5, 5, 7, 5, 5, 5, 5, 5, 9, 5, 5, 5, 5, 5, 5, 
7, 7, 7, 7, 7, 5, 9, 5, 5, 5, 5, 5, 7, 5, 5, 7, 5, 5, 5, 5, 5, 9, 5, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 7, 5, 9, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 9, 5, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 7, 5, 9, 5, 5, 7, 5, 5, 5, 6, 6, 5, 5, 5, 7, 5, 5, 9, 5, 7, 7, 7, 7, 7, 
5, 5, 5, 5, 5, 5, 9, 5, 5, 7, 5, 5, 7, 7, 7, 7, 5, 5, 7, 5, 5, 9, 5, 5, 5, 5, 5, 5, 
7, 7, 7, 7, 7, 7, 9, 7, 7, 7, 5, 5, 7, 7, 7, 7, 5, 5, 7, 7, 7, 9, 7, 7, 7, 7, 7, 7, 
5, 5, 5, 5, 5, 5, 9, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 7, 5, 5, 9, 5, 5, 5, 5, 5, 5, 
7, 7, 7, 7, 7, 5, 9, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 7, 5, 5, 9, 5, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 7, 5, 9, 5, 5, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 9, 5, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 7, 5, 9, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 7, 5, 5, 9, 5, 7, 7, 7, 7, 7, 
5, 5, 5, 5, 5, 5, 9, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 7, 5, 5, 9, 5, 5, 5, 5, 5, 5, 
5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 5, 5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 5, 
5, 9, 5, 5, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 5, 5, 9, 5, 
5, 9, 5, 5, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 9, 5, 5, 5, 5, 9, 5, 
5, 8, 9, 9, 5, 5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 5, 5, 9, 9, 8, 5, 
5, 5, 5, 9, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 9, 5, 5, 5, 
5, 5, 5, 9, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 9, 5, 5, 5, 
5, 9, 9, 9, 9, 9, 9, 5, 5, 9, 9, 9, 9, 5, 5, 9, 9, 9, 9, 5, 5, 9, 9, 9, 9, 9, 9, 5, 
5, 9, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 9, 5, 
5, 9, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 9, 5, 5, 9, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 9, 5, 
5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 5, 
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5]

const colors = ['magenta', 'lime', 'red', 'yellow']

class Map
{
	constructor(width, height)
	{
		this.width = 28
		this.height = 31
		this.cell_width = width / this.width
		this.cell_height = height / this.height
		this.food_count = 246
		this.buffer = default_map.slice()

		for(let i = 0; i < 6; i++)
		{
			let x = parseInt(Math.random() * (this.width - 1))
			let y = parseInt(Math.random() * (this.height - 1))

			if(this.buffer[y * this.width + x] == NORMAL_FOOD)
				this.buffer[y * this.width + x] = BIG_FOOD
		}
	}

	at(x, y)
	{
		x = parseInt(x)
		y = parseInt(y)

		if(x >= this.width || x < 0) return NONE
		if(y >= this.height || y < 0) return NONE

		return this.buffer[y * this.width + x]
	}

	pat(x, y, value = NONE)
	{
		x = parseInt(x)
		y = parseInt(y)

		if(x >= this.width || x < 0) return
		if(y >= this.height || y < 0) return

		this.buffer[y * this.width + x] = value
	}

	draw(ctx, color = 'blue')
	{
		const lw = this.cell_width
		const lh = this.cell_height

		const ldw = lw * 10 / 29
		const ldh = lh * 10 / 29

		for(let x = 0; x < this.width; x++)
		{
			for(let y = 0; y < this.height; y++)
			{
				const lx = x * lw;
				const ly = y * lh;

				switch(this.at(x, y))
				{
					case NONE:
						break;

					case WALL:
						{
							ctx.beginPath()
							ctx.lineWidth = 2
							ctx.strokeStyle = color

							let c = 1;

							if(this.at(x + 1, y) != WALL)
							{
								ctx.moveTo(lx + lw - ldw, ly + (this.at(x, y - 1) != WALL ? ldh : 0))
								ctx.lineTo(lx + lw - ldw, ly + lh - (this.at(x, y + 1) != WALL ? ldh : 0))
							}
							else
								c *= 2;

							if(this.at(x - 1, y) != WALL)
							{
								ctx.moveTo(lx + ldw, ly + (this.at(x, y - 1) != WALL ? ldh : 0))
								ctx.lineTo(lx + ldw, ly + lh - (this.at(x, y + 1) != WALL ? ldh : 0))
							}
							else
								c *= 3;

							if(this.at(x, y + 1) != WALL)
							{
								ctx.moveTo(lx + (this.at(x - 1, y) != WALL ? ldw : 0), ly + lh - ldh)
								ctx.lineTo(lx + lw - (this.at(x + 1, y) != WALL ? ldw : 0), ly + lh - ldh)
							}
							else
								c *= 5;

							if(this.at(x, y - 1) != WALL)
							{
								ctx.moveTo(lx + (this.at(x - 1, y) != WALL ? ldw : 0), ly + ldh)
								ctx.lineTo(lx + lw - (this.at(x + 1, y) != WALL ? ldw : 0), ly + ldh)
							}
							else
								c *= 7;

							if(c % 21 == 0)
							{
								ctx.moveTo(lx + ldw, ly)
								ctx.lineTo(lx, ly + ldh)
							}
							if(c % 14 == 0)
							{
								ctx.moveTo(lx + lw - ldw, ly)
								ctx.lineTo(lx + lw, ly + ldh)
							}
							if(c % 10 == 0)
							{
								ctx.moveTo(lx + lw - ldw, ly + lh)
								ctx.lineTo( lx + lw, ly + lh - ldh)
							}
							if(c % 15 == 0)
							{
								ctx.moveTo(lx, ly + lh - ldh)
								ctx.lineTo(lx + ldw, ly + lh)
							}

							ctx.stroke();
						}
						break;

					case DOOR:
						{
							ctx.beginPath()
							ctx.lineWidth = 2
							ctx.strokeStyle = '#ffeeee'

							ctx.moveTo(lx, ly + ldh)
							ctx.lineTo(lx + lw, ly + ldh)

							ctx.stroke();
						}
						break;

					case NORMAL_FOOD:
						{
							ctx.fillStyle = '#ffffae'
							ctx.fillRect(lx + 5 * lw/12, ly + 5 * lh/12, lw/6, lh/6);
						}
						break;

					case BIG_FOOD:
						{
							ctx.fillStyle = colors[parseInt(performance.now() / 100) % color.length]
							ctx.fillRect(lx + lw * 0.3, ly + lh * 0.3, lw * 0.4, lh * 0.4);
						}
						break;
				}
			}
		}
	}
}

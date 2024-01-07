class Pacman extends Entity
{
	static RAD(a)
	{
		return Math.PI * a / 180;
	}

	constructor(map)
	{
		super(
			12.5 * map.cell_width,
			23.5 * map.cell_height,
			map.cell_width - 1,
			map.cell_height - 1,
			map.cell_width / 5,
			map.cell_height / 5)

		this.score = 0
	}

	draw(ctx, color)
	{
		ctx.beginPath()
		ctx.fillStyle = 'yellow'
		ctx.arc(this.x, this.y, this.w * 4 / 5, 0, 2 * Math.PI)
		ctx.fill()

		const angle = Pacman.RAD(parseInt(performance.now()/16) % 55)
		const tx = Math.tan(angle);

		const w = tx * this.w/2
		const h = tx * this.h/2

		ctx.beginPath()
		ctx.fillStyle = 'black'

		switch(this.direction)
		{
			case RIGHT:
				ctx.moveTo(this.x + this.w, this.y + h)
				ctx.lineTo(this.x - 5, this.y)
				ctx.lineTo(this.x + this.w, this.y - h)
				break;

			case LEFT:
				ctx.moveTo(this.x - this.w, this.y - w)
				ctx.lineTo(this.x + 5, this.y)
				ctx.lineTo(this.x - this.w, this.y + w)
				break;

			case DOWN:
				ctx.moveTo(this.x + h, this.y + this.h)
				ctx.lineTo(this.x, this.y - 5)
				ctx.lineTo(this.x - h, this.y + this.h)
				break;

			case UP:
				ctx.moveTo(this.x - h, this.y - this.h)
				ctx.lineTo(this.x, this.y + 5)
				ctx.lineTo(this.x + h, this.y - this.h)
				break;
		}

		ctx.closePath();
		ctx.fill()
	}

	eat(map)
	{
		const x = this.x / map.cell_width;
		const y = this.y / map.cell_height;

		const k = map.at(x, y);

		if(k == BIG_FOOD || k == NORMAL_FOOD)
		{
			map.pat(parseInt(x), parseInt(y), NONE);
			this.score++;
		}

		return k == BIG_FOOD;
	}
}

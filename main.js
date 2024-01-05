const WIDTH  = 560
const HEIGHT = 602

const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')

let game_loop = true
let pause = false

let StartTime = 0
let FrameTime = 0

let Dt = 0

let map =  null
let pacman = null

let blinky = null
let pinky = null
let clyde = null
let inky = null

let wait_time = 0
let lives = 3

// For now this is fine
init()

const IId = window.requestAnimationFrame(main_loop);

let party_frames = 0
let deady_frames = 0

function main_loop(time)
{
	Dt = performance.now() - FrameTime
	FrameTime = performance.now()

	if(!pause)
	{
		old_move = pacman.direction
		pacman.direction = move
		if(!pacman.move(map))
		{
			pacman.direction = old_move
			pacman.move(map)
		}

		if(party_frames != 0 && deady_frames != 0)
		{
			update()
		}
		else
		{
			if(party_frames > 0)
			{
				if(party_frames == 1)
				{
					init()
					wait_time++
				}
				else
					party_frames--
			}
			else
			{
				if(deady_frames == 1)
				{
					init()
					if(lives > 0)
						lives--;
					else
					{
						lives = 3
						wait_time = 0
					}
				}
				deady_frames--
			}
		}
	}

	render()

	if(game_loop)
		window.requestAnimationFrame(main_loop);
}

function current_time()
{
	return performance.now() - StartTime
}

function init()
{
	map = new Map(WIDTH, HEIGHT)
	pacman = new Pacman(map)

	let sdf = wait_time >=  7 ? 0 :  7 - wait_time;

	blinky = new Ghost(14.5, 14.5, map, Ghost.blinky_target, sdf)
	pinky = new Ghost(13.5, 14.5, map, Ghost.pinky_target, sdf + 0.001)
	clyde = new Ghost(14.5, 13.5, map, Ghost.clyde_target, sdf + 0.003)
	inky = new Ghost(13.5, 13.5, map, Ghost.inky_target, sdf + 0.002)

	pacman.direction = NO_MOVE
	StartTime = performance.now()
}

const map_colors = ['deepskyblue', 'dogerblue', 'cornflowerblue', 'royalblue', 'slateblue', 'mediumslateblue', 'aquamarine']
function render()
{
	// Clearing rect
	ctx.fillStyle = 'black'
	ctx.fillRect(0, 0, WIDTH, HEIGHT)

	pacman.draw(ctx);

	if(party_frames > 0)
		map.draw(ctx, ['green', 'lightgreen', 'limegreen', 'white'][party_frames % 4]);
	else if(deady_frames > 0)
		map.draw(ctx, ['crimson', 'orangered', 'red', 'white'][deady_frames % 4]);
	else
		map.draw(ctx, map_colors[wait_time > map_colors.length - 1 ? map_colors.length - 1 : wait_time]);

	draw_lives()

	blinky.draw(ctx, 'red');
	pinky.draw(ctx, 'pink');
	clyde.draw(ctx, 'orange');
	inky.draw(ctx, 'cyan');

	if(pause)
	{
		ctx.fillStyle = '#ff0000'
		ctx.fillRect(10, 10, 10, 20);
		ctx.fillRect(25, 10, 10, 20);

		ctx.beginPath()
		ctx.strokeStyle = '#ff0000'
		ctx.lineWidth = 5
		ctx.moveTo(WIDTH - 10, 10)
		ctx.lineTo(WIDTH - 30, 30)
		ctx.moveTo(WIDTH - 30, 10)
		ctx.lineTo(WIDTH - 10, 30)
		ctx.stroke()
	}
}

let old_move = 0
let move = 0

function update()
{	
	if(pacman.eat(map))
	{
		const k = (g) => {
			if(g.state == CHASE || g.state == SCATTER)
			{
				g.prev_state = g.state
				g.state = FRIGHTENTED
				g.state_timer = current_time()
			}
		}

		k(blinky)
		k(pinky)
		k(inky)
		k(clyde)
	}

	if(pacman.score == map.food_count)
	{
		party_frames = 100
	}

	if(blinky.update(map, pacman)
		|| pinky.update(map, pacman)
		|| inky.update(map, pacman, blinky)
		|| clyde.update(map, pacman))
	{
//		deady_frames = 100
	}
}

addEventListener('keydown', (e) => {	
	switch(e.keyCode)
	{
		case 37: move = LEFT; break;
		case 38: move = UP; break;
		case 39: move = RIGHT; break;
		case 40: move = DOWN; break;

		case 32: pause = !pause; break;

		case 27: game_loop = false;
	}
}, false)

let startX = 0
let startY = 0

addEventListener('touchstart', (e) => {
	e.preventDefault()
	startX = e.touches[0].clientX
	startY = e.touches[0].clientY
}, false)

addEventListener('touchend', (e) => {
	e.preventDefault()
	let dx = e.changedTouches[0].clientX - startX
	let dy = e.changedTouches[0].clientY - startY

	if(Math.abs(dx) > Math.abs(dy))
	{
		if(dx > 0)
			move = RIGHT
		else
			move = LEFT
	}
	else
	{
		if(dy > 0)
			move = DOWN
		else
			move = UP
	}
}, false)

function draw_lives()
{
	const tx = 0.5;

	for(let i = 0; i < lives; i++)
	{
		ctx.fillStyle = 'black'
		ctx.fillRect(12.5 * map.cell_width + i * map.cell_width,
			15 * map.cell_width, map.cell_width, map.cell_height + 2)

		ctx.beginPath()
		ctx.fillStyle = 'yellow'
		ctx.arc(13 * map.cell_width + i * map.cell_width, 15.5 * map.cell_width, map.cell_height/2 + 1, 0, 2 * Math.PI)
		ctx.fill()

		ctx.beginPath()
		ctx.fillStyle = 'black'
		ctx.moveTo(13 * map.cell_width + i * map.cell_width, 15.5 * map.cell_width)
		ctx.lineTo(13 * map.cell_width + i * map.cell_width + map.cell_height/2 + 1, 15.5 * map.cell_width + map.cell_height/2 + 1)
		ctx.lineTo(13 * map.cell_width + i * map.cell_width + map.cell_height/2 + 1, 15.5 * map.cell_width - map.cell_height/2 + 1)
		ctx.fill()
	}
}

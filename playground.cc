#include "playground.h"
#include "player.h"
#include "screen.h"

bool Playground::create(Arena& _arena, Layout& layout, unsigned short _players){
	if ( _players < 2)
		return false;
	else {
		arena = &_arena;
		layout.setup(*this, _players);
		_arena.background();
		_arena.foreground();
		players = _players;
		return true;
	}
}

bool Playground::fire(unsigned short x, unsigned short y, unsigned short player){
	cell &c = field[y][x];
	switch (c.type){
		case CELL_GRASS:
		case CELL_ITEM:
			c.value = 0;
			c.type = CELL_FIRE;
			// no break
		case CELL_FIRE:
			c.player = player;
			c.tick = TICK_FIRE;
			break;
		case CELL_BOMB:
			explode(x, y, c.extra, player); // points for player initiating the explosion only
			break;
		case CELL_BLOCK:
			// TODO: Player points
			c.type = CELL_BLOCKONFIRE;
			c.player = player;
			c.tick = TICK_BLOCKONFIRE;
			// no break
		case CELL_BLOCKONFIRE:
		case CELL_WALL:
			return false;
	}
	return true;
}

void Playground::explode(unsigned short x, unsigned short y, unsigned short power, unsigned short player){
	cell &c = field[y][x];
	c.value = 0;
	c.type = CELL_FIRE;
	c.player = player;
	c.tick = TICK_FIRE;
	for (unsigned short _x = x - 1; _x >= max<unsigned short>(0, x - power); _x--)
		if (!fire(_x, y, player))
			break;
	for (unsigned short _x = x + 1; _x <= min<unsigned short>(width - 1, x + power); _x++)
		if (!fire(_x, y, player))
			break;
	for (unsigned short _y = y - 1; _y >= max<unsigned short>(0, y - power); _y--)
		if (!fire(x, _y, player))
			break;
	for (unsigned short _y = y + 1; _y <= min<unsigned short>(height - 1, y + power); _y++)
		if (!fire(x, _y, player))
			break;
}


void Playground::tick(){
	bool status = false;
	bool hasExplodingBomb = false;
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++){
			cell &c = field[y][x];
			if (--c.tick == 0)
				switch (c.type){
					case CELL_BOMB:
						hasExplodingBomb = true;
						status = true;
						break;
					case CELL_BLOCKONFIRE:
						c.player = 0;
						c.type = c.extra == 0 ? CELL_GRASS : CELL_ITEM;
						status = true;
						break;
					case CELL_FIRE:
						c.value = 0;
						c.type = CELL_GRASS;
						status = true;
						break;
					default: break;
				}
			}

	if (hasExplodingBomb)
		for (unsigned short y = 0; y < playground.height; y++)
			for (unsigned short x = 0; x < playground.width; x++){
				cell &c = field[y][x];
				if (c.type == CELL_BOMB && c.tick == 0)
					explode(x, y, c.extra, c.player);
			}
	if (status)
		arena->update();
}

void Playground::draw(){
	screen.lock();

	arena->draw();

	for (int p = 0; p < players; p++){
		player[p].draw();
	}

	screen.unlock();
	screen.flip();

}

Playground playground;


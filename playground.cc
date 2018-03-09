#include <iostream>
#include <cassert>  

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

void Playground::killall(unsigned short x, unsigned short y, unsigned short _player){
	for (int p = 0; p < players; p++){
		unsigned short px, py;
		player[p].getPos(px, py);
		if (px == x && py == y){
			if (p != _player)
				player[_player].addPoint(Player::POINT_KILL_PLAYER);
			player[p].die();
		}
	}
}

void Playground::access(unsigned short x, unsigned short y, unsigned short _player){
	// Check cells
	cell &c = field[y][x];
	switch (c.type){
		case CELL_GRASS:
			break;
		case CELL_ITEM:
			player[_player].item((enum ItemType) c.extra);
			c.value = 0;
			c.type = CELL_GRASS;
			arena->update();
			break;
		case CELL_FIRE:
			if (c.player != _player)
				player[c.player].addPoint(Player::POINT_KILL_PLAYER);
			player[_player].die();
			break;

		case CELL_BOMB:
			break;
		default:
			// error
			std::cerr << "Should not happen!" << c.type << std::endl;
	}
}


bool Playground::fire(unsigned short x, unsigned short y, unsigned short _player){
	// Check Player
	killall(x, y, _player);

	// Check cells
	cell &c = field[y][x];
	switch (c.type){
		case CELL_ITEM:
			player[_player].addPoint(Player::POINT_DESTROY_ITEM);
			// no break
		case CELL_GRASS:
			c.value = 0;
			c.type = CELL_FIRE;
			// no break
		case CELL_FIRE:
			c.player = _player;
			c.tick = TICK_FIRE;
			break;
		case CELL_BOMB:
			player[_player].addPoint(Player::POINT_BOMB_CHAIN);
			explode(x, y, _player); // points for player initiating the explosion only
			break;
		case CELL_BLOCK:
			player[_player].addPoint(Player::POINT_DESTROY_BLOCK);
			c.type = CELL_BLOCKONFIRE;
			c.player = _player;
			c.tick = TICK_BLOCKONFIRE;
			// no break
		case CELL_BLOCKONFIRE:
		case CELL_WALL:
			return false;
	}
	return true;
}

void Playground::explode(unsigned short x, unsigned short y, unsigned short _player){
	cell &c = field[y][x];
	assert(c.type == CELL_BOMB);
	player[(int)c.player].item(ITEM_BOMB);
	unsigned short power = c.extra;
	c.value = 0;
	c.type = CELL_FIRE;
	c.player = _player;
	c.tick = TICK_FIRE;
	for (unsigned short _x = x - 1; _x >= max<unsigned short>(0, x - power); _x--)
		if (!fire(_x, y, _player))
			break;
	for (unsigned short _x = x + 1; _x <= min<unsigned short>(width - 1, x + power); _x++)
		if (!fire(_x, y, _player))
			break;
	for (unsigned short _y = y - 1; _y >= max<unsigned short>(0, y - power); _y--)
		if (!fire(x, _y, _player))
			break;
	for (unsigned short _y = y + 1; _y <= min<unsigned short>(height - 1, y + power); _y++)
		if (!fire(x, _y, _player))
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
				if (c.type == CELL_BOMB && c.tick == 0){
					killall(x, y, c.player);
					explode(x, y, c.player);
				}
			}

	if (status)
		arena->update();

	draw(true);
}

void Playground::draw(bool tick){
	screen.lock();

	arena->draw(tick);

	for (int p = 0; p < players; p++){
		player[p].draw(tick);
	}

	screen.unlock();
	screen.flip();

}

Playground playground;


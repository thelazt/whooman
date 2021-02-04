#include "playground.h"
#include "player.h"
#include "screen.h"
#include "item.h"
#include "utils.h"


bool Playground::create(Arena * _arena, Layout& layout, enum Item::ItemSet itemset, unsigned short _players) {
	if (_players < 2) {
		return false;
	} else {
		state = GAME_ACTIVE;
		players = _players;
		arena = _arena;
		layout.setup(_players);
		Item::distribute(itemset);

		arena->create();

		for (unsigned short y = 0; y < playground.height; y++)
			for (unsigned short x = 0; x < playground.width; x++)
				dangerzone[y][x] = 0;
		return true;
	}
}

void Playground::killall(unsigned short x, unsigned short y, unsigned short _player) {
	for (int p = 0; p < players; p++) {
		unsigned short px, py;
		player[p].getPos(px, py);
		if (px == x && py == y) {
			if (p != _player)
				player[_player].addPoint(Player::POINT_KILL_PLAYER);
			player[p].die();
		}
	}
}

void Playground::access(unsigned short x, unsigned short y, unsigned short _player) {
	// Check cells
	cell &c = field[y][x];
	switch (c.type) {
		case CELL_GRASS:
			break;
		case CELL_ITEM:
			player[_player].item((enum Item::ItemType) c.extra);
			c.value = 0;
			c.type = CELL_GRASS;
			arena->update();
			break;
		case CELL_FIRE:
		case CELL_BLOCKONFIRE:
			if (c.player != _player)
				player[c.player].addPoint(Player::POINT_KILL_PLAYER);
			player[_player].die();
			break;
		case CELL_BOMB:
			// we could still be ON the bomb we've just planted
			break;
		default:
			// error
			DBG << "Should not happen!" << c.type << endl;
	}
}


bool Playground::fire(unsigned short x, unsigned short y, unsigned short _player) {
	// Check Player
	killall(x, y, _player);

	// Check cells
	cell &c = field[y][x];
	switch (c.type) {
		case CELL_ITEM:
			player[_player].addPoint(Player::POINT_DESTROY_ITEM);
			[[gnu::fallthrough]];
		case CELL_GRASS:
			c.value = 0;
			c.type = CELL_FIRE;
			[[gnu::fallthrough]];
		case CELL_FIRE:
			c.player = _player;
			c.tick = TICK_FIRE;
			return true;
		case CELL_BOMB:
			player[_player].addPoint(Player::POINT_BOMB_CHAIN);
			explode(x, y, _player);  // points for player initiating the explosion only
			return true;
		case CELL_BLOCK:
			player[_player].addPoint(Player::POINT_DESTROY_BLOCK);
			c.type = CELL_BLOCKONFIRE;
			c.player = _player;
			c.tick = TICK_BLOCKONFIRE;
			[[gnu::fallthrough]];
		default:
			return false;
	}
}

void Playground::explode(unsigned short x, unsigned short y, unsigned short _player) {
	cell &c = field[y][x];
	assert(c.type == CELL_BOMB);
	auto & cplayer = player[static_cast<int>(c.player)];
	cplayer.item(Item::ITEM_BOMB);
	cplayer.wait = false;
	cplayer.cover = false;
	unsigned short power = c.extra;
	c.value = 0;
	c.type = CELL_FIRE;
	c.player = _player;
	c.tick = TICK_FIRE;
	for (unsigned short _x = x - 1; _x >= max<short>(1, ((short)x) - power); _x--)
		if (!fire(_x, y, _player))
			break;
	for (unsigned short _x = x + 1; _x <= min<unsigned short>(width - 2, x + power); _x++)
		if (!fire(_x, y, _player))
			break;
	for (unsigned short _y = y - 1; _y >= max<short>(1, ((short)y) - power); _y--)
		if (!fire(x, _y, _player))
			break;
	for (unsigned short _y = y + 1; _y <= min<unsigned short>(height - 2, y + power); _y++)
		if (!fire(x, _y, _player))
			break;
}

void Playground::reevaluate() {
	// Reset
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++)
			dangerzone[y][x] = 0;
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++) {
			cell &c = field[y][x];
			switch (c.type) {
				case CELL_BOMB:
					dangerous(x, y, c.extra);
					break;
				case CELL_FIRE:
				case CELL_BLOCKONFIRE:
					dangerzone[y][x]++;
					break;
				default:
					break;
			}
		}
}

bool Playground::dangerous(unsigned short x, unsigned short y) {
	// Check cells
	cell &c = field[y][x];
	switch (c.type) {
		case CELL_ITEM:
		case CELL_GRASS:
		case CELL_FIRE:
		case CELL_BLOCKONFIRE:
		case CELL_BOMB:
			dangerzone[y][x]++;
			return true;
		default:
			return false;
	}
}

void Playground::dangerous(unsigned short x, unsigned short y, unsigned short power) {
	dangerzone[y][x]++;
	for (unsigned short _x = x - 1; _x >= max<short>(1, ((short)x) - power); _x--)
		if (!dangerous(_x, y))
			break;
	for (unsigned short _x = x + 1; _x <= min<unsigned short>(width - 2, x + power); _x++)
		if (!dangerous(_x, y))
			break;
	for (unsigned short _y = y - 1; _y >= max<short>(1, ((short)y) - power); _y--)
		if (!dangerous(x, _y))
			break;
	for (unsigned short _y = y + 1; _y <= min<unsigned short>(height - 2, y + power); _y++)
		if (!dangerous(x, _y))
			break;
}

bool Playground::bomb(unsigned short x, unsigned short y, unsigned short _player,
                      unsigned short power, unsigned short ticks) {
	cell &c = field[y][x];
	if (c.type == CELL_GRASS) {
		c.type = CELL_BOMB;
		c.tick = ticks;
		c.player = _player;
		c.extra = power;
		// Mark dangerzone
		dangerous(x, y, power);
		return true;
	} else {
		return false;
	}
}

bool Playground::danger(unsigned short x, unsigned short y) {
	return dangerzone[y][x] > 0;
}

bool Playground::accessible(unsigned short x, unsigned short y, enum PlaygroundAccess access) {
	switch (access) {
		case ACCESS_SAFE:
			if (dangerzone[y][x] > 0)
				return false;
			[[gnu::fallthrough]];
		case ACCESS_DANGEROUS:
			if (field[y][x].type == CELL_FIRE || field[y][x].type == CELL_BLOCKONFIRE)
				return false;
			[[gnu::fallthrough]];
		default:
			return (field[y][x].type & CELL_ACCESSIBLE) != 0;
	}
}

enum Playground::GameState Playground::check() {
	if (state == GAME_ACTIVE) {
		int survivors = 0;
		int last = -1;
		for (int p = 0; p < playground.playerCount(); p++)
			if (player[p].alive) {
				survivors++;
				last = p;
			}
		if (survivors == 1) {
			player[last].win();
			state = GAME_WON;
		} else if (survivors == 0) {
			state = GAME_DRAW;
		}
	}
	return state;
}

enum Playground::GameState Playground::tick() {
	bool status = false;
	bool hasExplodingBomb = false;
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++) {
			cell &c = field[y][x];
			if (--c.tick == 0)
				switch (c.type) {
					case CELL_BOMB:
						hasExplodingBomb = true;
						status = true;
						break;
					case CELL_BLOCKONFIRE:
						c.player = 0;
						c.type = c.extra == 0 ? CELL_GRASS : CELL_ITEM;
						reevaluate();
						status = true;
						break;
					case CELL_FIRE:
						c.value = 0;
						c.type = CELL_GRASS;
						if (dangerzone[y][x] > 0)
							dangerzone[y][x]--;
						status = true;
						break;
					default: break;
				}
			}

	if (state == GAME_ACTIVE && hasExplodingBomb) {
		for (unsigned short y = 0; y < playground.height; y++) {
			for (unsigned short x = 0; x < playground.width; x++) {
				cell &c = field[y][x];
				if (c.type == CELL_BOMB && c.tick == 0) {
					killall(x, y, c.player);
					explode(x, y, c.player);
				}
			}
		}
	}
	check();

	if (status)
		arena->update();

	for (int p = 0; p < players; p++)
		player[p].tick();

	return state;
}

void Playground::draw(bool tick) {
	arena->draw();

	for (int p = 0; p < players; p++)
		player[p].draw(tick);
}

Playground playground;

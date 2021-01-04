#include "arena.h"
#include "utils.h"
#include "player.h"

Arena::Arena(unsigned short _offsetX, unsigned short _offsetY, unsigned short _tileSize, const char * groundSprite,
             const unsigned short blockAni, const char * bombSprite, const char * fireSprite, const char * itemSprite,
             const char * statsSprite)
             : ground(groundSprite, _tileSize, _tileSize),
               bomb(bombSprite, _tileSize, _tileSize),
               fire(fireSprite, _tileSize, _tileSize),
               item(itemSprite, _tileSize, _tileSize),
               stats(statsSprite, defaultStatsWidth, defaultStatsHeight),
               blockAni(blockAni),
               offsetX(_offsetX), offsetY(_offsetY), tileSize(_tileSize) {}

void Arena::draw() {
	// statusbar
	for (short p = 0; p < 4 ; p++)
		statusbar(p);
	for (unsigned short y = 0; y < playground.getHeight(); y++)
		for (unsigned short x = 0; x < playground.getWidth(); x++) {
			cell &c = playground.get(x, y);
			unsigned short _x = x * tileSize + offsetX;
			unsigned short _y = y * tileSize + offsetY;

			ground.draw(c.surface + (c.type == CELL_BLOCK ? c.tick % blockAni : 0), _x, _y);

			switch (c.type) {
				case CELL_BLOCKONFIRE:
					 {
						unsigned short sprite = TICK_FIRE - c.tick;
						if (sprite == 0)
							sprite = 10;
						else if (sprite > 6)
							break;
						else
							sprite += 3;
						ground.draw(sprite, _x, _y);
					 }
					break;
				case CELL_ITEM:
					item.draw((c.tick % 2) * 5 + c.extra - 1, _x, _y);
					break;
				case CELL_BOMB:
					bomb.draw(((c.tick % 4) == 3 ? 1 : (c.tick % 4)) + (c.tick <= 3 ? 3 : 0), _x, _y);
					break;
				case CELL_FIRE:
					fire.draw(((c.tick % 9) > 4 ? (8 - (c.tick % 9)) : c.tick) * 7 + c.sprite , _x, _y);
					break;
				default:
					break;
			}
		}
}

unsigned short Arena::fireSprite(unsigned short x, unsigned short y) {
	bool up, down, left, right;
	unsigned short count = 0;
	count += (up = (playground.get(x, y - 1).type & CELL_ONFIRE)) ? 1 : 0;
	count += (down = (playground.get(x, y + 1).type & CELL_ONFIRE)) ? 1 : 0;
	count += (left = (playground.get(x - 1, y).type & CELL_ONFIRE)) ? 1 : 0;
	count += (right = (playground.get(x + 1, y).type & CELL_ONFIRE)) ? 1 : 0;
	switch (count) {
		case 1:
			if (down)
				return 0;
			else if (left)
				return 1;
			else if (up)
				return 2;
			else if (right)
				return 3;
			assert(false);
			break;
		case 0:
			DBG << "invalid fire" << count <<  endl;
			[[gnu::fallthrough]];
		case 2:
			if (up && down)
				return 4;
			else if (left && right)
				return 5;
			[[gnu::fallthrough]];
		default:
			return 6;
	}
}

void Arena::update() {
	for (unsigned short y = 1; y < playground.getHeight() - 1; y++)
		for (unsigned short x = 1; x < playground.getWidth() - 1; x++) {
			cell &c = playground.get(x, y);
			switch (c.type) {
				case CELL_WALL:
					c.surface = 3;
					break;
				case CELL_BLOCK:
					c.surface = 10;
					break;
				case CELL_FIRE:
					c.sprite = fireSprite(x, y);
				[[gnu::fallthrough]];
				default:
					if (y <= 0)
						c.surface = 0;
					else
						switch (playground.get(x, y-1).type) {
							case CELL_WALL:
								c.surface = 1;
								break;
							case CELL_BLOCK:
								c.surface = 2;
								break;
							default:
								c.surface = 0;
						}
			}
		}
}

unsigned short Arena::decorate(short x, short y) {
	if (x >= 0 && x < playground.getWidth() && y <= playground.getHeight())
		return y < playground.getHeight() ? 3 : 1;
	else
		return 0;
}

void Arena::statusbar(short p) {
	short screenPart = Screen::getWidth() / playground.playerCount();
	short start = screenPart * p;
	short y = offsetY - defaultStatsHeight;
	for (short x = 0; x < screenPart; x += defaultStatsWidth)
		stats.draw(12, x + start, y);
	stats.draw(10, defaultStatsWidth * 3 + start, y);
	stats.draw(11, defaultStatsWidth * 11 + start, y);

	player[p].skin->draw(player[p].isAlive() ? 0 : 5, start, y - 1, 0, 36);

	unsigned int sum = player[p].getPoints();
	for (short q = 10; q > 3 ; q--) {
		stats.draw(sum % 10, defaultStatsWidth * q + start, y);
		sum /= 10;
	}
}

void Arena::create() {
	// statusbar
	for (short p = 0; p < 4 ; p++)
		statusbar(p);
	// outside
	for (short y = 0; offsetY + y * tileSize < (short)(Screen::getHeight() + tileSize); y++) {
		for (short x = -1; offsetX + x * tileSize > -((short)tileSize); x--)
			ground.draw(decorate(x, y), offsetX + x * tileSize, offsetY + y * tileSize);
		if (y >= playground.getHeight()) {
			for (short x = 0; x < playground.getWidth(); x++)
				ground.draw(decorate(x, y), offsetX + x * tileSize, offsetY + y * tileSize);
		}
		for (short x = playground.getWidth(); offsetX + x * tileSize < (short)(Screen::getWidth() + tileSize); x++)
			ground.draw(decorate(x, y), offsetX + x * tileSize, offsetY + y * tileSize);
	}
	// Border
	for (unsigned short y = 0; y < playground.getHeight(); y++) {
		playground.get(0, y).surface = decorate(0, y);
		playground.get(playground.getWidth() - 1, y).surface = decorate(playground.getWidth() - 1, y);
	}
	for (unsigned short x = 0; x < playground.getWidth(); x++) {
		playground.get(x, 0).surface = decorate(x, 0);
		playground.get(x, playground.getHeight() - 1).surface = decorate(x, playground.getHeight() - 1);
	}
	// Foreground
	update();
}

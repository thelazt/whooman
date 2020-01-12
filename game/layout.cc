#include "layout.h"
#include "player.h"

void Layout::defaultLayout() {
	for (unsigned short y = 0; y < playground.getHeight(); y++)
		for (unsigned short x = 0; x < playground.getWidth(); x++) {
			cell& c = playground.get(x, y);
			c.value = 0;
			// Outer Walls
			if (y == 0 || x == 0 || y == playground.getHeight()-1 || x == playground.getWidth() - 1)
				c.type = CELL_WALL;
			// Inner Walls
			else if (y % 2  == 0 && x % 2 == 0 )
				c.type = CELL_WALL;
			// Blocks
			else if ((x > 2 && x < playground.getWidth() - 3) || (y > 2 && y < playground.getHeight() - 3))
				c.type = CELL_BLOCK;
			// Grass
			else
				c.type = CELL_GRASS;
		}
}

void Layout::defaultPlayer(unsigned short players) {
	for (int p = 0; p < players; p++)
		player[p].init(p < 2 ? 1 : (playground.getWidth() - 2), p % 2 ? 1 : (playground.getHeight() - 2));
}

void Layout::setup(unsigned short players) {
	defaultLayout();
	defaultPlayer(players);
}

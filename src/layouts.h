#pragma once

#include "layout.h"

class LayoutClassic : public Layout {
 public:
	void setup(unsigned short players) {
		defaultLayout();
		defaultPlayer(players);
	}
};

class LayoutPatchy : public Layout {
 public:
	void setup(unsigned short players) {
		defaultLayout();
		for (unsigned short y = 1; y < playground.getHeight() - 1; y++)
			for (unsigned short x = 1; x < playground.getWidth() - 1; x++) {
				cell& c = playground.get(x, y);
				if (c.type == CELL_WALL && number() % 5 == 0)
					playground.get(x, y).type = CELL_BLOCK;
				else if (c.type == CELL_BLOCK && number() % 10 == 0)
						playground.get(x, y).type = CELL_GRASS;
			}
		defaultPlayer(players);
	}
};

class LayoutRandomStart : public Layout {
 public:
	void setup(unsigned short players) {
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
				else
					c.type = CELL_BLOCK;
			}
		for (int p = 0; p < players; p++) {
			unsigned short px = number() % (playground.getWidth() - 4) + 2;
			unsigned short py = number() % (playground.getHeight() - 4) + 2;
			playground.get(px, py).type = CELL_GRASS;
			playground.get(px - 1, py).type = CELL_GRASS;
			playground.get(px + 1, py).type = CELL_GRASS;
			playground.get(px, py - 1).type = CELL_GRASS;
			playground.get(px, py + 1).type = CELL_GRASS;
			player[p].init(px, py);
		}
	}
};

class LayoutDense : public Layout {
 public:
	void setup(unsigned short players) {
		for (unsigned short y = 0; y < playground.getHeight(); y++)
			for (unsigned short x = 0; x < playground.getWidth(); x++) {
				cell& c = playground.get(x, y);
				c.value = 0;
				// Outer Walls
				if (y == 0 || x == 0 || y == playground.getHeight()-1 || x == playground.getWidth() - 1)
					c.type = CELL_WALL;
				// Grass
				else if ((x < 3 || x > playground.getWidth() - 4) && (y < 3 || y > playground.getHeight() - 4))
					c.type = CELL_GRASS;
				// Inner Walls
				else if ((y+1) % 3 <= 1 && (x+1) % 3 <= 1 )
					c.type = CELL_WALL;
				// Blocks
				else
					c.type = CELL_BLOCK;
			}
		defaultPlayer(players);
	}
};

class LayoutSparse : public Layout {
 public:
	void setup(unsigned short players) {
		for (unsigned short y = 0; y < playground.getHeight(); y++)
			for (unsigned short x = 0; x < playground.getWidth(); x++) {
				cell& c = playground.get(x, y);
				c.value = 0;
				// Outer Walls
				if (y == 0 || x == 0 || y == playground.getHeight()-1 || x == playground.getWidth() - 1)
					c.type = CELL_WALL;
				// Grass
				else if ((x < 3 || x > playground.getWidth() - 4) && (y < 3 || y > playground.getHeight() - 4))
					c.type = CELL_GRASS;
				// Inner Walls
				else if (y % 3 == 0 && x % 3 == 1)
					c.type = CELL_WALL;
				// Blocks
				else
					c.type = CELL_BLOCK;
			}
		defaultPlayer(players);
	}
};

class LayoutEmpty : public Layout {
 public:
	void setup(unsigned short players){
		for (unsigned short y = 0; y < playground.getHeight(); y++)
			for (unsigned short x = 0; x < playground.getWidth(); x++) {
				cell& c = playground.get(x, y);
				c.value = 0;
				// Outer Walls
				if (y == 0 || x == 0 || y == playground.getHeight()-1 || x == playground.getWidth() - 1)
					c.type = CELL_WALL;
				// Blocks
				else if ((x > 2 && x < playground.getWidth() - 3) || (y > 2 && y < playground.getHeight() - 3))
					c.type = CELL_BLOCK;
				// Grass
				else
					c.type = CELL_GRASS;
			}
		defaultPlayer(players);
	}
};

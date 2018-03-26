#ifndef _BOMB_LAYOUTS_H
#define _BOMB_LAYOUTS_H

#include "layout.h"

class LayoutEmpty : public Layout {
public:
	void setup(unsigned short players){
		for (unsigned short y = 0; y < playground.getHeight(); y++)
			for (unsigned short x = 0; x < playground.getWidth(); x++){
				cell& c = playground.get(x,y);
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

#endif

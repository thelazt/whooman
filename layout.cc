#include "layout.h"
#include "player.h"

void Layout::defaultLayout(Playground& ground){
	for (unsigned short y = 0; y < ground.height; y++)
		for (unsigned short x = 0; x < ground.width; x++){
			cell& c = ground.get(x,y);
			c.value = 0;
			// Outer Walls
			if (y == 0 || x == 0 || y == ground.height-1 || x == ground.width - 1)
				c.type = CELL_WALL;
			// Inner Walls
			else if (y % 2  == 0 && x % 2 == 0 )
				c.type = CELL_WALL;
			// Blocks
			else if ((x > 2 && x <ground. width - 3) || (y > 2 && y < ground.height - 3)){
				c.type = CELL_BLOCK;
				// TODO: Item
			}
			// Grass
			else 
				c.type = CELL_GRASS;
		}
}

void Layout::defaultPlayer(Playground& ground, unsigned short players){
	for (int p = 0; p < players; p++)
		player[p].reset(p < 2 ? 1 : (ground.width - 2), p % 2 ? 1 : (ground.height - 2));
}

void Layout::setup(Playground& ground, unsigned short players){
	defaultLayout(ground);
	defaultPlayer(ground, players);
}


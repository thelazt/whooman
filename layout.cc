#include "layout.h"
#include "player.h"

struct {
	enum ItemType item;
	unsigned short probability;
	unsigned short limit;
} distribution[] = {
	{ ITEM_BOMB, 10, 10 },
	{ ITEM_POWER, 10, 10 },
	{ ITEM_SPEED, 4, 10 },
	{ ITEM_SICK, 3, 3 },
	{ ITEM_ULTRA, 1, 2 },
};


void Layout::defaultLayout(Playground& ground){
	for (unsigned short y = 0; y < ground.getHeight(); y++)
		for (unsigned short x = 0; x < ground.getWidth(); x++){
			cell& c = ground.get(x,y);
			c.value = 0;
			// Outer Walls
			if (y == 0 || x == 0 || y == ground.getHeight()-1 || x == ground.getWidth() - 1)
				c.type = CELL_WALL;
			// Inner Walls
			else if (y % 2  == 0 && x % 2 == 0 )
				c.type = CELL_WALL;
			// Blocks
			else if ((x > 2 && x < ground.getWidth() - 3) || (y > 2 && y < ground.getHeight() - 3))
				c.type = CELL_BLOCK;
			// Grass
			else 
				c.type = CELL_GRASS;
		}
}

void Layout::defaultPlayer(Playground& ground, unsigned short players){
	for (int p = 0; p < players; p++)
		player[p].reset(p < 2 ? 1 : (ground.getWidth() - 2), p % 2 ? 1 : (ground.getHeight() - 2));
}

void Layout::defaultItems(Playground& ground){
	struct xy {
		unsigned short x, y; 
	};
	// Save all CELL_BLOCK
	struct xy block[ground.getHeight() * ground.getWidth()];
	unsigned short blocks = 0;
	for (unsigned short y = 0; y < ground.getHeight(); y++)
		for (unsigned short x = 0; x < ground.getWidth(); x++)
			if (ground.get(x,y).type == CELL_BLOCK)
				block[blocks++] = { x, y };
	// Shuffle
	for (unsigned short b = blocks - 1; b > 0; b--){
		int i = number() % b;
		cell &c = ground.get(block[i].x, block[i].y);
		int j = number() % 100;
		for(int d = 0; d < 5; d++)
			if (j < distribution[d].probability){
				if (distribution[d].limit > 0){
					c.extra = distribution[d].item;
					distribution[d].limit--;
				}
				break;
			} else
				j -= distribution[d].probability;
		block[i] = block[b];
	}
}

void Layout::setup(Playground& ground, unsigned short players){
	defaultLayout(ground);
	defaultPlayer(ground, players);
	defaultItems(ground);
}


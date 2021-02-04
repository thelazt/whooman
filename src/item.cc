#include "item.h"
#include "def.h"
#include "playground.h"
#include "utils.h"

void Item::distribute(struct ItemProbability distribution) {
	struct xy {
		unsigned short x, y;
	};
	// Save all CELL_BLOCK
	struct xy block[playground.getHeight() * playground.getWidth()];
	unsigned short blocks = 0;
	for (unsigned short y = 0; y < playground.getHeight(); y++)
		for (unsigned short x = 0; x < playground.getWidth(); x++)
			if (playground.get(x, y).type == CELL_BLOCK)
				block[blocks++] = { x, y };
	// Shuffle
	for (unsigned short b = blocks - 1; b > 0; b--) {
		int i = number() % b;
		cell &c = playground.get(block[i].x, block[i].y);
		int j = number() % 100;
		for(enum ItemType d = ITEM_BOMB; d <= ITEM_SICK; d = (enum ItemType) (d + 1))
			if (j < distribution.item[d].probability) {
				if (distribution.item[d].limit > 0) {
					c.extra = d;
					distribution.item[d].limit--;
				}
				break;
			} else {
				j -= distribution.item[d].probability;
			}
		block[i] = block[b];
	}
}

struct Item::ItemProbability Item::getSet(enum Item::ItemSet name) {
	switch (name) {
		case SET_BASIC:
			return (struct Item::ItemProbability) {{
				{ 0, 0}, 		// ITEM_NONE (ignored)
				{ 10, 10 }, 	// ITEM_BOMB
				{ 0, 0 },		// ITEM_SPEED
				{ 10, 10 },		// ITEM_POWER
				{ 0, 0 },		// ITEM_ULTRA
				{ 0, 0 },		// ITEM_SICK
			}};  //NOLINT
		case SET_FAST:
			return (struct Item::ItemProbability) {{
				{ 0, 0}, 		// ITEM_NONE (ignored)
				{ 10, 15 }, 	// ITEM_BOMB
				{ 3, 8 },		// ITEM_SPEED
				{ 8, 10 },		// ITEM_POWER
				{ 2, 5 },		// ITEM_ULTRA
				{ 1, 3 },		// ITEM_SICK
			}};  //NOLINT
		case SET_RANDOM:
			return getSet((enum ItemSet) (number() % SET_RANDOM));
		default:
			return (struct Item::ItemProbability) {{
				{ 0, 0}, 		// ITEM_NONE (ignored)
				{ 10, 10 }, 	// ITEM_BOMB
				{ 4, 10 },		// ITEM_SPEED
				{ 10, 10 },		// ITEM_POWER
				{ 1, 2 },		// ITEM_ULTRA
				{ 13, 13 },		// ITEM_SICK
			}};  //NOLINT
	}
}

void Item::distribute(enum ItemSet itemset) {
	distribute(getSet(itemset));
}

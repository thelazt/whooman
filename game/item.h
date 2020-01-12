#pragma once

class Item {
 public:
	enum ItemSet { SET_BASIC, SET_CLASSIC, SET_RANDOM };
	enum ItemType { ITEM_NONE = 0, ITEM_BOMB = 1, ITEM_SPEED = 2, ITEM_POWER = 3, ITEM_ULTRA = 4, ITEM_SICK = 5};
	static void distribute(enum ItemSet itemset);

 private:
	struct ItemProbability {
		struct {
			unsigned short probability;
			unsigned short limit;
		} item[6];
	};

	static void distribute(struct ItemProbability distribution);
	static struct ItemProbability getSet(enum ItemSet itemset);
};

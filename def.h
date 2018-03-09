#ifndef _BOMB_DEF_H
#define _BOMB_DEF_H

const unsigned defaultTileSize = 48;

const unsigned maxPlayer = 4;

enum ItemType { ITEM_BOMB = 0, ITEM_SPEED = 1, ITEM_POWER = 2, ITEM_ULTRA = 3, ITEM_SICK = 4};

const unsigned short TICK_FIRE = 8;
const unsigned short TICK_BLOCKONFIRE = 7;
const unsigned short TICK_BOMB = 16;

const unsigned CELL_ACCESSABLE = 0x80;
const unsigned CELL_ONFIRE = 0x40;
enum CellType {
	CELL_WALL = 0,
	CELL_BLOCK = 1,
	CELL_BOMB = 2,
	CELL_FIRE = CELL_ACCESSABLE | CELL_ONFIRE | 3,
	CELL_BLOCKONFIRE = CELL_ACCESSABLE | CELL_ONFIRE | 4,
	CELL_ITEM = CELL_ACCESSABLE | 5,
	CELL_GRASS = CELL_ACCESSABLE | 0xf,
};

typedef union {
	struct {
		enum CellType type  : 8;
		unsigned short surface : 2;	// Used by Arena only
		unsigned short sprite  : 6;	// Used by Arena only
		unsigned short tick : 8;
		unsigned short player : 2;
		unsigned short extra : 6;
		/*
			Block: Item
			Bomb: Power
			Item: Item
		*/
	};
	unsigned int value;
} cell;


template <class T> const T& max (const T& a, const T& b) {
	return (a<b)?b:a;
}

template <class T> const T& min (const T& a, const T& b) {
	return (a>b)?b:a;
}

extern int number();
#endif

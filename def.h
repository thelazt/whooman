#ifndef _BOMB_DEF_H
#define _BOMB_DEF_H

const unsigned defaultTileSize = 48;

const unsigned maxPlayer = 4;

enum ItemType { ITEM_BOMB, ITEM_SPEED, ITEM_POWER, ITEM_SICK, ITEM_ULTRA };

const unsigned CELL_ACCESSABLE = 0xf0;
enum CellType {
	CELL_WALL = 0,
	CELL_BLOCK = 1,
	CELL_BOMB = 2,
	CELL_FIRE = CELL_ACCESSABLE | 0,
	CELL_ITEM = CELL_ACCESSABLE | 1,
	CELL_GRASS = CELL_ACCESSABLE | 2
};

typedef union {
	struct {
		enum CellType type  : 8;
		unsigned int sprite : 8;
		unsigned int player : 2;
		unsigned int tick : 6;
		unsigned int extra : 8;
		/*
			Wall: Sprit Type
			Block: Item ?
			Bomb: Time:8 + Power:6
			Fire: Sprite Type
			Item: Type
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


#endif

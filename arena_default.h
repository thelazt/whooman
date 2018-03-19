#ifndef _BOMB_ARENA_DEFAULT_H
#define _BOMB_ARENA_DEFAULT_H

#include "arena.h"

class ArenaDefault: public Arena {
public:
	ArenaDefault(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_default.png") {
	};
};

class ArenaAtomic: public Arena {
public:
	ArenaAtomic(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_atomic.png", 1, "img/atomic_bomb.png", "img/atomic_fire.png", "img/atomic_items.png") {
	};
};

class ArenaRetro: public Arena {
public:
	ArenaRetro(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_retro.png", 1, "img/retro_bomb.png", "img/retro_fire.png", "img/retro_items.png") {
	};
};

class ArenaCastle: public Arena {
public:
	ArenaCastle(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_castle.png", 10) {
	};

	unsigned short decorate(short x, short y){
		if (x >= 0 && x < playground.width && y < playground.height){
			if (y == 0){
				if (x == 0)
					return 21;
				else if (x == playground.width - 1)
					return 26;
				else
					return 20;
			} else if (y == playground.height - 1){
				if (x == 0)
					return 24;
				else if (x == playground.width - 1)
					return 29;
				else
					return 25;
			} else if (x == 0)
				return 22;
			else
				return 28;
		} else
			return 30;
	}
};


class ArenaTest: public Arena {
public:
	ArenaTest(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_castle.png", 10) {
	};

	unsigned short decorate(short x, short y){
		if (x >= 0 && x < playground.width && y < playground.height){
			if (y == 0){
				if (x == 0)
					return 21;
				else if (x == playground.width - 1)
					return 26;
				else
					return 20;
			} else if (y == playground.height - 1){
				if (x == 0)
					return 24;
				else if (x == playground.width - 1)
					return 29;
				else
					return 25;
			} else if (x == 0)
				return 22;
			else
				return 28;
		} else
			return 30;
	}
};



#endif

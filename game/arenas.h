#ifndef _BOMB_ARENAS_H
#define _BOMB_ARENAS_H

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
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_retro.png", 1, "img/retro_bomb.png", "img/retro_fire.png", "img/retro_items.png", "img/retro_stats.png") {
	};
};

class ArenaCastle: public Arena {
public:
	ArenaCastle(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_castle.png", 10) {
	};

	unsigned short decorate(short x, short y){
		if (x >= 0 && x < playground.getWidth() && y < playground.getHeight()){
			if (y == 0){
				if (x == 0)
					return 21;
				else if (x == playground.getWidth() - 1)
					return 26;
				else
					return 20;
			} else if (y == playground.getHeight() - 1){
				if (x == 0)
					return 24;
				else if (x == playground.getWidth() - 1)
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


class ArenaCity: public Arena {
public:
	ArenaCity(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_city.png", 4) {
	};

	unsigned short decorate(short x, short y){
		unsigned short i = 0;
		if (x < 0)
			i = 19;
		else if (x >= playground.getWidth())
			i = 26;
		else if (y == playground.getHeight())
			i = 1; 
		if (y < playground.getHeight()){
			if (x == 0)
				i =  16;
			else if (x == playground.getWidth() - 1)
				i =  23;
		} else {
			if (x == -1)
				i =  21;
			else if (x == playground.getWidth())
				i =  28;
		}
		if (y == playground.getHeight() - 1 && x >= -1 && x<= playground.getWidth()){
			if (x == 0)
				i = 17;
			else if (x == playground.getWidth() - 1)
				i = 24;
			else if (x == -1)
				i = 20;
			else if (x == playground.getWidth())
				i = 27;
			else
				i = 29;
		}
		else if (y == 0 && x >= 0 && x < playground.getWidth()){ // Top
			if (x == 0)
				i = 15; // Left Corner
			else if (x == 1)
				i = 18;
			else if (x == playground.getWidth() - 2)
				i = 25;
			else if (x == playground.getWidth() - 1)
				i = 22; // Right Corner
			else
				i = 14; // Line
		}
		return i;
	}
};

class ArenaDirt: public Arena {
public:
	ArenaDirt(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_dirt.png", 1) {
	};

	unsigned short decorate(short x, short y){
		if (y == 0 || y == playground.getHeight() - 1){
			if (x == 0 || x == playground.getWidth() - 1)
				return 12;
			else if (x > 0 && x < playground.getWidth())
				return x % 2 == 1 ? 11 : 15;
		} else if (y < playground.getHeight() && (x== 0 || x == playground.getWidth() - 1))
			return y % 2 == 1 ? 13: 14;
		return 0;
	}
};

class ArenaFactory: public Arena {
public:
	ArenaFactory(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_factory.png", 12) {
	};

	unsigned short decorate(short x, short y){
		if (y == 0){
			if (x == 0)
				return 23;
			else if (x == playground.getWidth() - 1)
				return 26;
			else 
				return 22;
		} else if (y == playground.getHeight() - 1 ){
			if (x == 0)
				return 25;
			else if (x == playground.getWidth() - 1)
				return 28;
			else if (x > 0 && x < playground.getWidth())
				return 29;
		}
		if (x == 0)
			return 24;
		else if (x == playground.getWidth() - 1)
			return 27;
		return 0;
	}
};

class ArenaHAL: public Arena {
public:
	ArenaHAL(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) : 
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_hal.png", 1) {
	};

	unsigned short decorate(short x, short y){
		if (x >= 0 && x < playground.getWidth() && y < playground.getHeight()){
			if (y == 0){
				if (x == 0)
					return 12;
				else if (x == playground.getWidth() - 1)
					return 15;
				else
					return 11;
			} else if (y == playground.getHeight() - 1){
				if (x == 0)
					return 14;
				else if (x == playground.getWidth() - 1)
					return 17;
				else
					return 11;
			} else if (x == 0)
				return 13;
			else
				return 16;
		} else if (y == playground.getHeight() && x >= 0 && x < playground.getWidth())
			return 1;
		else
			return 0;
	}
};

class ArenaStation: public Arena {
public:
	ArenaStation(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) :
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_station.png", 5) {
	};

	unsigned short decorate(short x, short y){
		if (x == 0)
			return y % 2  ? 17 : 16;
		else if (x == -1)
			return y % 2  ? 19 : 18;
		else if (x < -1)
			return y % 2  ? 21 : 20;

		if (x == playground.getWidth() - 1)
			return y % 2  ? 23 : 22;
		else if (x == playground.getWidth())
			return y % 2  ? 25 : 24;
		else if (x > playground.getWidth() )
			return y % 2  ? 27 : 26;

		if (y == 0)
			return 15;
		else if (y == playground.getHeight() - 1)
			return 28;
		else if (y == playground.getHeight())
			return 29;
		else
			return 0;
	}
};

class ArenaWood: public Arena {
public:
	ArenaWood(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) :
		Arena(_offsetX, _offsetY, _tileSize, "img/playground_wood.png", 1) {
	};

	unsigned short decorate(short x, short y){
		if (x < -1 || x > playground.getWidth() || y > playground.getHeight())
			return 0;

		if (x == 0){
			if (y == 0)
				return 12;
			else if (y == 1)
				return 13;
			else if (y == playground.getHeight() - 2)
				return 15;
			else if (y == playground.getHeight() - 1)
				return 16;
			else if (y == playground.getHeight())
				return 11;
			else
				return 14;
		} else if (x == -1){
			if (y == 0)
				return 18;
			else if (y == playground.getHeight() - 2)
				return 20;
			else if (y == playground.getHeight() - 1)
				return 21;
			else if (y == playground.getHeight())
				return 22;
			else
				return 19;
		} else if (x == playground.getWidth() - 1){
			if (y == 0)
				return 23;
			else if (y == 1)
				return 24;
			else if (y == playground.getHeight() - 2)
				return 26;
			else if (y == playground.getHeight() - 1)
				return 27;
			else if (y == playground.getHeight())
				return 11;
			else
				return 25;
		} else if (x == playground.getWidth()){
			if (y == 0)
				return 28;
			else if (y == playground.getHeight() - 2)
				return 30;
			else if (y == playground.getHeight() - 1)
				return 31;
			else if (y == playground.getHeight())
				return 32;
			else
				return 29;
		}
		else if (y == playground.getHeight() - 1)
			return 17;
		else if (y == 0 || y == playground.getHeight())
			return 11;
		else
			return 0;
	}
};



#endif

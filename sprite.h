#ifndef _BOMB_SPRITE_H
#define _BOMB_SPRITE_H

#include "SDL/SDL.h"

#include "def.h"

class Sprite {
private:
	SDL_Surface * img;
	unsigned short width;
	unsigned short height;
	static bool init;

public:
	Sprite (const char * path = nullptr, unsigned short w = defaultTileSize, unsigned short h = defaultTileSize);

	void load(const char * path, unsigned short w = defaultTileSize, unsigned short h = defaultTileSize);

	void draw(unsigned short num, short x, short y);
};

#endif

#pragma once

#include "SDL/SDL.h"

#include "def.h"

class Sprite {
 private:
	SDL_Surface * img;
	unsigned short width;
	unsigned short height;
	static bool init;

 public:
	explicit Sprite(const char * path = nullptr, unsigned short w = defaultTileSize, unsigned short h = defaultTileSize);

	void load(const char * path, unsigned short w = defaultTileSize, unsigned short h = defaultTileSize);

	void draw(unsigned short num, short x, short y, unsigned short w = 0, unsigned short h = 0);
};

#pragma once

#include "def.h"
#include "utils/png.h"
#include "screen.h"

class Sprite {
 private:
	PNG image;
	unsigned short width;
	unsigned short height;

 public:
	explicit Sprite(const char * path = nullptr, unsigned short w = defaultTileSize, unsigned short h = defaultTileSize)
	                : image(path), width(w), height(h) {}

	inline void draw(unsigned short num, short x, short y, unsigned short w = 0, unsigned short h = 0) {
		graphics.image(Point(x, y), image,
		               w == 0 ? width : w, h == 0 ? height : h,
		               0, (num * height));
	}
};

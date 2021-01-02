#pragma once

#include "SDL/SDL.h"

namespace Screen {
	extern SDL_Surface *content;

	void init(unsigned int _width, unsigned int _height, const char * title = "Bomberman");

	unsigned int getWidth();

	unsigned int getHeight();

	void lock();

	void unlock();

	void block();

	void unblock();

	void flip(bool force = false);

	void deinit();
}  // namespace Screen

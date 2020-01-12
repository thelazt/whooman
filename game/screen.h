#pragma once

#include "SDL/SDL.h"

class Screen {
 public:
	const unsigned int width;
	const unsigned int height;
	SDL_Surface *content;

	Screen(unsigned int _width, unsigned int _height, const char * title = "Bomberman");

	void lock();

	void unlock();

	void flip();

	~Screen();
};

extern Screen screen;

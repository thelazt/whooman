#include "screen.h"

namespace Screen {
static unsigned int width;
static unsigned int height;
SDL_Surface *content;

void init(unsigned int _width, unsigned int _height, const char * title) {
	width = _width;
	height = _height;
	SDL_Init(SDL_INIT_EVERYTHING);
	content = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption(title, NULL);
	SDL_FillRect(content, NULL, SDL_MapRGB(content->format, 0, 0, 0));
}

unsigned int getWidth() {
	return width;
}

unsigned int getHeight() {
	return height;
}
void lock() {
	if (SDL_MUSTLOCK(content)) {
		SDL_LockSurface(content);
	}
}

void unlock() {
	if (SDL_MUSTLOCK(content)) {
		SDL_UnlockSurface(content);
	}
}

void flip() {
	SDL_Flip(content);
}

void deinit() {
	SDL_Quit();
}

}  // namespace Screen

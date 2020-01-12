#include "screen.h"

Screen::Screen(unsigned int _width, unsigned int _height, const char * title) : width(_width), height(_height) {
	SDL_Init(SDL_INIT_EVERYTHING);
	content = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption(title, NULL);
	SDL_FillRect(content, NULL, SDL_MapRGB(content->format, 0, 0, 0));
}

void Screen::lock() {
	if (SDL_MUSTLOCK(content)) {
		SDL_LockSurface(content);
	}
}

void Screen::unlock() {
	if (SDL_MUSTLOCK(content)) {
		SDL_UnlockSurface(content);
	}
}

void Screen::flip() {
	SDL_Flip(content);
}

Screen::~Screen() {
	SDL_Quit();
}

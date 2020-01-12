#include <iostream>
#include "SDL/SDL_image.h"

#include "sprite.h"

#include "screen.h"


bool Sprite::init = false;

Sprite::Sprite (const char * path, unsigned short w, unsigned short h){
	if (!init && !( IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG ) ){
		std::cout << "SDL_image could not initialize! SDL_image Error: " <<IMG_GetError() <<std::endl;
		exit(1);
	}
	init = true;
	load(path,w,h);
}

void Sprite::load(const char * path, unsigned short w, unsigned short h){
	width = w;
	height = h;

	if (path == nullptr)
		img = nullptr;
	else {
		img = IMG_Load (path );
		if (!img){
			std::cout <<  "IMG_Load Error: " <<  IMG_GetError () << std::endl;
			exit(1);
		}
	}
}

void Sprite::draw(unsigned short num, short x, short y, unsigned short w, unsigned short h){
	SDL_Rect rcSrc = { 0, (short) (num * height), w == 0 ? width : w, h == 0 ? height : h};
	SDL_Rect rcDest = { x, y, w == 0 ? width : w, h == 0 ? height : h};
	int r = SDL_BlitSurface ( img, &rcSrc, screen.content, &rcDest );
	if (r != 0){
		std::cout << "SDL_BlitSurface Error: " <<IMG_GetError() << std::endl;
		exit(1);
	}
}


#ifndef _BOMB_ARENA_H
#define _BOMB_ARENA_H

class Arena;
#include "def.h"
#include "sprite.h"
#include "playground.h"
#include "screen.h"

class Arena {
protected:
	Sprite ground, bomb, fire, item;

	const unsigned short blockAni;
	unsigned short fireSprite(unsigned short x, unsigned short y);

	Arena(unsigned short _offsetX, unsigned short _offsetY, unsigned short _tileSize, const char * groundSprite, const unsigned short blockAni = 1, const char * bombSprite = "img/default_bomb.png", const char * fireSprite = "img/default_fire.png", const char * itemsSprite="img/default_items.png");


public:
	const unsigned short offsetX, offsetY;
	const unsigned short tileSize;

	// TODO: Statusbar

	void draw(bool tick = false);	// ticks might have changed

	void update();	// Items have changed

	virtual unsigned short decorate(short x, short y);

	void create();
};

#endif

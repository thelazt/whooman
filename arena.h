#ifndef _BOMB_ARENA_H
#define _BOMB_ARENA_H

class Arena;
#include "def.h"
#include "sprite.h"
#include "playground.h"

class Arena {
protected:
	Sprite bomb, fire, item, ground, danger;

	unsigned short fireSprite(unsigned short x, unsigned short y);

public:
	const unsigned short offsetX, offsetY;
	const unsigned short tileSize;

	Arena(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize);

	// TODO: Statusbar

	virtual void draw(bool tick = false);	// ticks might have changed

	virtual void update();	// Items have changed

	virtual void background();

	virtual void foreground();
};

#endif

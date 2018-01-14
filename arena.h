#ifndef _BOMB_ARENA_H
#define _BOMB_ARENA_H

class Arena;
#include "def.h"
#include "sprite.h"
#include "playground.h"

class Arena {
protected:
	Sprite bomb;
	Sprite fire;
	Sprite item;
	Sprite ground;

public:
	const unsigned short offsetX;
	const unsigned short offsetY;
	const unsigned short tileSize;

	Arena(unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize);

	// TODO: Statusbar

	virtual void draw();

	virtual void background();

	virtual void foreground();
};

#endif

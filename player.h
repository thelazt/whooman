#ifndef _BOMB_PLAYER_H
#define _BOMB_PLAYER_H

class Player;
#include "def.h"
#include "sprite.h"
#include "playground.h"

class Player {
public:
	enum PlayerDir { MOVE_UP = 3, MOVE_DOWN = 0, MOVE_LEFT = 1, MOVE_RIGHT = 2, MOVE_AUTO = 0xf, MOVE_HEAVEN = 0xf0, MOVE_WON = 0xf1, MOVE_WAIT = 0xff};
	enum PlayerPoints { 
		POINT_SURVIVE       = 100000,
		POINT_KILL_PLAYER   = 10000,
		POINT_DESTROY_ITEM  = 1000,
		POINT_BOMB_CHAIN    = 100,
		POINT_DESTROY_BLOCK = 10,
		POINT_ALIVE         = 1
	};

	const unsigned short id;

private:
	unsigned short size;
	unsigned short figureSpace;
	unsigned short x;
	unsigned short y;
	short offsetX;
	short offsetY;
	short offsetXabs;
	short offsetYabs;
	unsigned short power;
	unsigned short bombs;
	unsigned short speed;
	unsigned short sickness;
	unsigned int points;
	
	bool alive;
	enum PlayerDir dir;
	unsigned int counter;
	unsigned short ani;
	unsigned short tileSize;

	// Bot specific
	unsigned short targetX, targetY;
	unsigned short previousX, previousY;

	Sprite skin;
	static unsigned short idCounter;

	const unsigned short factor = 2;

	bool shouldBomb(unsigned short _x, unsigned short _y);
	bool atPos(unsigned short x, unsigned short y);
	bool findTarget(unsigned short x, unsigned short y, enum Playground::PlaygroundAccess access);
	bool near(unsigned short _x,unsigned short _y);

protected:
	friend class Playground;
	bool wait, cover;

public:
	Player();

	void reset(unsigned short _x, unsigned short _y, unsigned short _tileSize = defaultTileSize);

	void load(const char * path, unsigned short _size = 64, unsigned short _figureSpace = 20, short _offsetX = -31, short _offsetY = -46);

	void getPos(unsigned short &_x,unsigned short &_y);

	void move(enum PlayerDir _dir = MOVE_AUTO);

	bool bomb();

	void addPoint(enum PlayerPoints event);

	unsigned int getPoints();

	void item(enum ItemType _item);

	void die();

	void draw(bool tick = false);
};

extern Player player[maxPlayer];

#endif

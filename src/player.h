#pragma once

class Player;
#include "def.h"
#include "sprite.h"
#include "playground.h"
#include "item.h"
#include "input.h"

class Player {
 public:

	enum PlayerDir {
		MOVE_UP = 3,
		MOVE_DOWN = 0,
		MOVE_LEFT = 1,
		MOVE_RIGHT = 2,
		MOVE_BOMB = 4,
		MOVE_AUTO = 0xf,
		MOVE_HEAVEN = 0xf0,
		MOVE_WON = 0xf1,
		MOVE_WAIT = 0xff
	};

	enum PlayerSickness {
		SICK_NONE,
		SICK_NOBOMB,
		SICK_NOPOWER,
		SICK_SLOW,
		SICK_INVISIBLE,
		SICK_FUZELONG,
		SICK_FUZESHORT,
		SICK_DROPBOMB
	};

	enum PlayerPoints {
		POINT_SURVIVE       = 100000,
		POINT_KILL_PLAYER   = 10000,
		POINT_DESTROY_ITEM  = 1000,
		POINT_BOMB_CHAIN    = 100,
		POINT_DESTROY_BLOCK = 10,
		POINT_ALIVE         = 1
	};

	Input::Method input;

	const unsigned short id;

	Sprite * skin = nullptr;

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
	enum PlayerSickness sickness;
	unsigned short sicknessTimer;
	unsigned short sicknessCounter;
	unsigned int points;

	bool alive;
	enum PlayerDir dir;
	unsigned int counter;
	unsigned short ani;
	unsigned short tileSize;

	// Bot specific
	unsigned short targetX, targetY;
	unsigned short previousX, previousY;

	static unsigned short idCounter;

	const unsigned short factor = 2;

	bool shouldBomb(unsigned short _x, unsigned short _y);
	bool atPos(unsigned short x, unsigned short y);
	bool findTarget(unsigned short x, unsigned short y, enum Playground::PlaygroundAccess access);
	bool near(unsigned short _x, unsigned short _y);

 protected:
	friend class Playground;
	bool wait, cover;

 public:
	Player();

	void init(unsigned short _x, unsigned short _y, unsigned short _tileSize = defaultTileSize);

	void load(const char * path, unsigned short _size = 64, unsigned short _figureSpace = 20,
	          short _offsetX = -31, short _offsetY = -46);

	void getPos(unsigned short &_x, unsigned short &_y);

	void move(enum PlayerDir _dir = MOVE_AUTO);

	bool bomb();

	void addPoint(enum PlayerPoints event);

	unsigned int getPoints();

	void resetPoints();

	bool isAlive() {
		return alive;
	}

	void item(enum Item::ItemType _item);

	void tick();

	void die();

	void win();

	void draw(bool tick = false);
};

extern Player player[maxPlayer];

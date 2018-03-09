#include "player.h"

#include <climits>
#include <iostream>

#include "playground.h"
#include "arena.h"

unsigned short Player::idCounter = 0;

Player::Player() : id(idCounter++){}

void Player::reset(unsigned short _x, unsigned short _y, unsigned short _tileSize){
	tileSize = _tileSize;
	x = (_x * tileSize + tileSize/2) << factor;
	y = (_y * tileSize + tileSize/2) << factor;
	offsetXabs = offsetX + playground.getArena().offsetX;
	offsetYabs = offsetY + playground.getArena().offsetY;
	points = 0;
	alive = true;
	power = 2;
	bombs = 1;
	speed = 1;
	sickness = 0;
	dir = MOVE_DOWN;
	ani = 1;
}

void Player::load(const char * path, unsigned short _size, unsigned short _figureSpace, short _offsetX, short _offsetY){
	offsetX = _offsetX;
	offsetY = _offsetY;
	size = _size;
	figureSpace = _figureSpace;
	skin = Sprite(path, _size, _size);
}


void Player::getPos(unsigned short &_x,unsigned short &_y){
	_x = (x >> factor) / tileSize;
	_y = (y >> factor) / tileSize;
}

void Player::move(enum PlayerDir _dir){
	static unsigned int counter = 0;
	if (alive){
		if (_dir == MOVE_AUTO){
			
		}
		// 
		short dirY = 0, dirX = 0;
		unsigned short s = (32 + speed);
		switch(_dir) {
			case MOVE_UP: dirY = -s; break;
			case MOVE_DOWN: dirY = s; break;
			case MOVE_LEFT: dirX = -s; break;
			case MOVE_RIGHT: dirX = s; break;
			default: break;
		}
		dir = _dir;
		if (counter++ % 2 == 0)
			ani++;
		unsigned short fx, fy;
		getPos(fx, fy);
		if (dirX <= 0)
			x = max<unsigned short>(x + dirX, playground.accessible(fx - 1, fy) ? 0 : ((fx * tileSize + figureSpace) << factor));
		if (dirX >= 0)
			x = min<unsigned short>(x + dirX, playground.accessible(fx + 1, fy) ? USHRT_MAX : (((fx + 1) * tileSize - figureSpace) << factor));
		if (dirY <= 0)
			y = max<unsigned short>(y + dirY, playground.accessible(fx, fy - 1) ? 0 : ((fy * tileSize + figureSpace) << factor));
		if (dirY >= 0)
			y = min<unsigned short>(y + dirY, playground.accessible(fx, fy + 1) ? USHRT_MAX : (((fy + 1) * tileSize - figureSpace) << factor));

		getPos(fx, fy);
		playground.access(fx, fy, id);
	}
}

void Player::bomb(){
	if (alive && bombs > 0){
		unsigned short fx, fy;
		getPos(fx, fy);
		if (playground.bomb(fx, fy, id, power, TICK_BOMB)){
			bombs--;
		}
	}
}

void Player::addPoint(enum PlayerPoints event){
	points+=event;
}

unsigned int Player::getPoints(){
	return points;
}

void Player::item(enum ItemType _item){
	switch(_item){
		case ITEM_BOMB:  bombs++; break;
		case ITEM_SPEED: speed++; break;
		case ITEM_POWER: power++; break;
		case ITEM_ULTRA: power+=10; break;
		case ITEM_SICK:
			sickness = 5; // TODO
			break;
	}
}

void Player::die(){
	if (alive){
		alive = false;
		dir = MOVE_HEAVEN;
		ani = 0;
		// TODO: Check survivors
	}
}

void Player::draw(bool tick){
	short num = -1;
	if (dir == MOVE_HEAVEN && ani < 3){
		num = ani;
		if (tick)
			ani++;
	} else if (dir == MOVE_WON){
		num = ani;
		if (tick)
			ani = (ani + 1) % 2;
	} else if (alive){
		ani %= 4;
		num = ani == 3 ? 1 : ani;
	}
	if (num >= 0)
		skin.draw(dir * 3 + num, (x >> factor) + offsetXabs, (y >> factor) + offsetYabs);
}

Player player[maxPlayer];


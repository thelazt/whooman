#include "player.h"

#include <climits>
#include <iostream>

#include "playground.h"

unsigned short Player::idCounter = 0;

Player::Player() : id(idCounter++){}

void Player::reset(unsigned short _x, unsigned short _y, unsigned short _tileSize){
	tileSize = _tileSize;
	x = _x * tileSize + tileSize/2;
	y = _y * tileSize + tileSize/2;
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
	_x = x;
	_y = y;
}

void Player::move(enum PlayerDir _dir){
	if (alive){
		short dirY = 0, dirX = 0;
		unsigned short speed = 1;
		switch(_dir) {
			case MOVE_UP: dirY = -speed; break;
			case MOVE_DOWN: dirY = speed; break;
			case MOVE_LEFT: dirX = -speed; break;
			case MOVE_RIGHT: dirX = speed; break;
			default: break;
		}
		dir = _dir;
		ani++;
		unsigned short fx = x/tileSize ;
		unsigned short fy = y/tileSize ;
		if (!(dirX > 0))
			x = max<unsigned short>(x + dirX, (playground.get(fx - 1, fy).type & CELL_ACCESSABLE) ? 0 : (fx * tileSize + figureSpace));
		if (!(dirX < 0))
			x = min<unsigned short>(x + dirX, (playground.get(fx + 1, fy).type & CELL_ACCESSABLE) ? USHRT_MAX : ((fx + 1) * tileSize - figureSpace));
		if (!(dirY > 0))
			y = max<unsigned short>(y + dirY, (playground.get(fx, fy - 1).type & CELL_ACCESSABLE) ? 0 : (fy * tileSize + figureSpace));
		if (!(dirY < 0))
			y = min<unsigned short>(y + dirY, (playground.get(fx, fy + 1).type & CELL_ACCESSABLE) ? USHRT_MAX : ((fy + 1) * tileSize - figureSpace));
	}
}

void Player::bomb(){
	if (alive){
		unsigned short fx = x/tileSize;
		unsigned short fy = y/tileSize;
		playground.get(fx, fy).type = CELL_BOMB;
	}
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
	alive = false;
	dir = MOVE_HEAVEN;
	ani = 0;
}

void Player::draw(){
	short num = -1;
	if (dir == MOVE_HEAVEN && ani <4){
		num = ani++;
	} else if (alive){
		ani %= 4;
		num = ani == 3 ? 1 : ani;
	}
	if (num >= 0)
		skin.draw(dir * 3 + num, x + offsetX, y + offsetY);
}

Player player[maxPlayer];


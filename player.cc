#include "player.h"

#include <climits>
#include <iostream>

#include "playground.h"
#include "arena.h"

unsigned short Player::idCounter = 0;

Player::Player() : id(idCounter++), counter(0) {}

void Player::reset(unsigned short _x, unsigned short _y, unsigned short _tileSize){
	tileSize = _tileSize;
	x = (_x * tileSize + tileSize/2) << factor;
	y = (_y * tileSize + tileSize/2) << factor;
	previousX = _x;
	previousY = _y;
	offsetXabs = offsetX + playground.getArena().offsetX;
	offsetYabs = offsetY + playground.getArena().offsetY;
	findTarget(_x,_y, Playground::ACCESS_DANGEROUS);
	wait = false;
	cover = false;
	points = 0;
	alive = true;
	power = 2;
	bombs = 1;
	speed = 1;
	sickness = 0;
	ani = 1;
}

void Player::load(const char * path, unsigned short _size, unsigned short _figureSpace, short _offsetX, short _offsetY){
	offsetX = _offsetX;
	offsetY = _offsetY;
	size = _size;
	figureSpace = _figureSpace;
	skin = Sprite(path, _size, _size);
}

bool Player::shouldBomb(unsigned short _x,unsigned short _y){
	if (playground.get(_x, _y).type == CELL_BLOCK && random() % 2)
		return true;
	for (int p = 0; p < playground.playerCount(); p++)
		if (p != id && player[p].atPos(_x, _y) && random() % 2)
			return true;
	return false;
}

bool Player::atPos(unsigned short _x,unsigned short _y){
	return alive && (_x == (x >> factor) / tileSize) && (_y == (y >> factor) / tileSize);
}

void Player::getPos(unsigned short &_x,unsigned short &_y){
	_x = (x >> factor) / tileSize;
	_y = (y >> factor) / tileSize;
}

bool Player::findTarget(unsigned short x, unsigned short y, enum Playground::PlaygroundAccess access){
	struct xyd {
		unsigned short x, y; 
		enum PlayerDir dir;
		void set(unsigned short _x, unsigned short _y, enum PlayerDir _dir){ x = _x; y = _y;  dir = _dir; };
	};
	struct xyd target[4];
	unsigned short targets = 0;
	if (playground.accessible(x + 1, y, access))
		target[targets++].set(x + 1, y, MOVE_RIGHT );
	if (playground.accessible(x - 1, y, access))
		target[targets++].set(x - 1, y, MOVE_LEFT);
	if (playground.accessible(x, y + 1, access))
		target[targets++].set(x, y + 1, MOVE_DOWN);
	if (playground.accessible(x, y - 1, access))
		target[targets++].set(x, y - 1, MOVE_UP);
	int i = 0;
	if (targets == 0)
		return false;
	else if (targets > 1){
		i = number() % targets;
		if (target[i].x == previousX && target[i].y == previousY){
			target[i] = target[targets - 1];
			i = number() % (targets - 1);
		}
	}
	previousX = x;
	previousY = y;
	targetX = target[i].x;
	targetY = target[i].y;
	dir = target[i].dir;
	return true;
}

bool Player::near(unsigned short _x,unsigned short _y){
	short tX = (_x * tileSize + tileSize/2) << factor;
	short tY = (_y * tileSize + tileSize/2) << factor;
	short limit = tileSize / 2;
	return (tX > x ? (tX - x) : (x - tX)) < limit && (tY > y ? (tY - y) : (y - tY)) < limit;
}


void Player::move(enum PlayerDir _dir){
	if (alive && _dir <= MOVE_AUTO){
		unsigned short fx, fy;
		getPos(fx, fy);
		if (_dir == MOVE_AUTO){
			if (near(targetX, targetY)){
				if (!cover && !playground.danger(fx,fy) && (shouldBomb(fx + 1, fy) || shouldBomb(fx - 1, fy) || shouldBomb(fx, fy + 1) || shouldBomb(fx, fy - 1)) && bomb())
					cover = true;
				if (cover && !playground.danger(fx,fy))
					wait = true;
				if (!wait && (number() % 3 != 0 || !findTarget(fx, fy, Playground::ACCESS_SAFE)))
					if (!findTarget(fx, fy, Playground::ACCESS_DANGEROUS))
						return;
			}
			if (wait){
				if (playground.danger(fx,fy) && (findTarget(fx, fy, Playground::ACCESS_SAFE) || findTarget(fx, fy, Playground::ACCESS_DANGEROUS)))
					wait = false;
				else
					return;
			}
		} else {
			dir = _dir;
		}

		short dirY = 0, dirX = 0;
		unsigned short s = (32 + speed);
		switch(dir) {
			case MOVE_UP: dirY = -s; break;
			case MOVE_DOWN: dirY = s; break;
			case MOVE_LEFT: dirX = -s; break;
			case MOVE_RIGHT: dirX = s; break;
			default: return;
		}
		if (counter++ % 2 == 0)
			ani++;

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

bool Player::bomb(){
	if (alive && bombs > 0){
		unsigned short fx, fy;
		getPos(fx, fy);
		if (playground.bomb(fx, fy, id, power, TICK_BOMB)){
			bombs--;
			return true;
		}
	}
	return false;
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
		playground.check();
	}
}

void Player::win(){
	dir = Player::MOVE_WON;
	ani = 0;
	alive = false;
	points += POINT_SURVIVE;
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


#include "arena.h"

#include <iostream>

Arena::Arena(unsigned short _offsetX, unsigned short _offsetY, unsigned short _tileSize) : bomb("img/default_bomb.png", _tileSize, _tileSize), fire("img/default_fire.png", _tileSize, _tileSize), item("img/default_items.png", _tileSize, _tileSize), ground("img/playground_default.png", _tileSize, _tileSize), offsetX(_offsetX), offsetY(_offsetY), tileSize(_tileSize) {
	}

void Arena::draw(bool tick){
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++){
			cell &c = playground.get(x, y);
			unsigned short _x = x * tileSize + offsetX;
			unsigned short _y = y * tileSize + offsetY;
			ground.draw(c.surface, _x, _y);
			switch (c.type){
				case CELL_BLOCKONFIRE:
					ground.draw(10 - c.tick, _x, _y);
					break;
				case CELL_ITEM:
					item.draw((c.tick % 2) * 5 + c.extra - 1, _x, _y);
					break;
				case CELL_BOMB:
					bomb.draw((c.tick % 4) == 3 ? 1 : (c.tick % 4), _x, _y);
					break;
				case CELL_FIRE:
					fire.draw(((c.tick % 9) > 4 ? (8 - (c.tick % 9)) : c.tick) * 9 + c.sprite , _x, _y);
				default: break;
			}
		}
}

unsigned short Arena::fireSprite(unsigned short x, unsigned short y){
	bool up, down, left, right;
	unsigned short count = 0; 
	count += up = playground.get(x, y - 1).type & CELL_ONFIRE;
	count += down = playground.get(x, y + 1).type & CELL_ONFIRE;
	count += left = playground.get(x - 1, y).type & CELL_ONFIRE;
	count += right = playground.get(x + 1, y).type & CELL_ONFIRE;
	switch (count){
		case 1:
			if (down)
				return 0;
			else if (left)
				return 1;
			else if (up)
				return 2;
			else if (right)
				return 3;
			// no break
		case 0:
			std::cout << "invalid fire" << std::endl;
			// no break
		case 2:
			if (up && down)
				return 4;
			else if (left && right)
				return 5;
			// no break;
		default:
			return 8;
	}
}

void Arena::update(){
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++){
			cell &c = playground.get(x, y);
			if (c.type == CELL_FIRE)
				c.sprite = fireSprite(x, y);
			// Surface
			switch (c.type){
				case CELL_WALL:
					c.surface = 2;
					break;
				case CELL_BLOCK:
					c.surface = 3;
					break;
				default: 
					c.surface = y > 0 && playground.get(x, y-1).type <= CELL_BLOCK? 1 : 0;
			}

		}
}

void Arena::background(){}

void Arena::foreground(){
	update();
}


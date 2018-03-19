#include "arena.h"

#include <cassert>
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
					fire.draw(((c.tick % 9) > 4 ? (6 - (c.tick % 7)) : c.tick) * 7 + c.sprite , _x, _y);
					break;
				default: break;
			}
		}
}

unsigned short Arena::fireSprite(unsigned short x, unsigned short y){
	bool up, down, left, right;
	unsigned short count = 0; 
	count += (up = (playground.get(x, y - 1).type & CELL_ONFIRE)) ? 1 : 0;
	count += (down = (playground.get(x, y + 1).type & CELL_ONFIRE)) ? 1 : 0;
	count += (left = (playground.get(x - 1, y).type & CELL_ONFIRE)) ? 1 : 0;
	count += (right = (playground.get(x + 1, y).type & CELL_ONFIRE)) ? 1 : 0;
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
			assert(false);
			break;
		case 0:
			std::cout << "invalid fire" << count <<  std::endl;
			[[gnu::fallthrough]];
		case 2:
			if (up && down)
				return 4;
			else if (left && right)
				return 5;
			[[gnu::fallthrough]];
		default:
			return 6;
	}
}

void Arena::update(){
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++){
			cell &c = playground.get(x, y);
			switch (c.type){
				case CELL_FIRE:
					c.sprite = fireSprite(x, y);
					break;
				case CELL_WALL:
					c.surface = 3;
					break;
				case CELL_BLOCK:
					c.surface = 10;
					break;
				default: 
					if (y > 0)
						switch (playground.get(x, y-1).type){
							case CELL_WALL:
								c.surface = 1;
								break;
							case CELL_BLOCK:
								c.surface = 2;
								break;
							default:
								c.surface = 0;
						}
			}
		}
}

void Arena::background(){}

void Arena::foreground(){
	update();
}


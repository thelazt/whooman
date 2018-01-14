#include "arena.h"

Arena::Arena(unsigned short _offsetX, unsigned short _offsetY, unsigned short _tileSize) : bomb("img/default_bomb.png", _tileSize, _tileSize), fire("img/default_fire.png", _tileSize, _tileSize), item("img/default_items.png", _tileSize, _tileSize), ground("img/playground_default.png", _tileSize, _tileSize), offsetX(_offsetX), offsetY(_offsetY), tileSize(_tileSize) {
	}

void Arena::draw(){
	int inc = 1;
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++){
			cell &c = playground.get(x, y);
			unsigned short _x = x * tileSize + offsetX;
			unsigned short _y = y * tileSize + offsetY;
			ground.draw(c.sprite, _x, _y);
			switch (c.type){
				case CELL_ITEM:
					c.tick = (c.tick + inc) % 2;
					item.draw(c.tick*9 + c.extra, _x, _y);
					break;
				case CELL_BOMB:
					c.tick+= inc;
					bomb.draw(c.tick % 4 == 3 ? 1 : c.tick % 4, _x, _y);
					break;
				case CELL_FIRE:
					c.tick = (c.tick + inc) % 9;
					fire.draw(c.tick > 4 ? (9 - c.tick) : c.tick, _x, _y);
				default: break;
			}
		}
}

void Arena::background(){}

void Arena::foreground(){
	for (unsigned short y = 0; y < playground.height; y++)
		for (unsigned short x = 0; x < playground.width; x++){
			cell &c = playground.get(x, y);
			switch (c.type){
				case CELL_WALL:
					c.sprite = 0;
					break;
				case CELL_BLOCK:
					c.sprite = 1;
					break;
				case CELL_GRASS:
					c.sprite = playground.get(x, y-1).type == CELL_WALL ? 8 : 9;;
					break;
				default: break;
			}
		}
}


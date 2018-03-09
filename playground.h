#ifndef _BOMB_PLAYGROUND_H
#define _BOMB_PLAYGROUND_H

class Playground;

#include "def.h"
#include "arena.h"
#include "layout.h"

class Playground {

private:
	static const unsigned short maxWidth = 21;
	static const unsigned short maxHeight = 16;

	cell field[maxHeight][maxWidth];
	Arena * arena;
	unsigned short players;

	void killall(unsigned short x, unsigned short y, unsigned short _player);
	bool fire(unsigned short x, unsigned short y, unsigned short _player);
	void explode(unsigned short x, unsigned short y, unsigned short power, unsigned short _player);

public:
	const unsigned short width;
	const unsigned short height;

	Playground(unsigned short _width = 15, unsigned short _height = 13) : width(_width > maxWidth ? maxWidth : _width), height(_height > maxHeight ? maxHeight : _height) {
	}

	void access(unsigned short x, unsigned short y, unsigned short _player);

	cell& get(unsigned short x, unsigned short y){
		return field[y][x];
	}

	bool accessible(unsigned short x, unsigned short y){
		return field[y][x].type & CELL_ACCESSABLE;
	}

	Arena& getArena(){
		return *arena;
	}

	bool create(Arena& _arena, Layout& layout, unsigned short _players = 4);

	void tick();
	void draw(bool tick = false);
};

extern Playground playground;

#endif

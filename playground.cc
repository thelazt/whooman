#include "playground.h"
#include "player.h"
#include "screen.h"

bool Playground::create(Arena * _arena, Layout& layout, unsigned short _players){
	if (_arena == nullptr || _players < 2)
		return false;
	else {
		layout.setup(*this, _players);
		arena = _arena;
		arena->background();
		arena->foreground();
		players = _players;
		return true;
	}
}

void Playground::draw(){
	screen.lock();

	arena->draw();

	for (int p = 0; p < players; p++){
		player[p].draw();
	}

	screen.unlock();
	screen.flip();

}

Playground playground;


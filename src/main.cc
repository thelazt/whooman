#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "screen.h"
#include "playground.h"
#include "layout.h"
#include "player.h"
#include "game.h"
#include "menu.h"

int main() {
	Screen::init(1024, 768);
	srand(time(NULL));
	Menu menu;

	menu.show();

/*	return 0;

	game.play(3, 4, 19, 15);
	// 19x15  gross
	// 15x13
	// 11x9  klein
	*/
	Screen::deinit();
	return 0;
}

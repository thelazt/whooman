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

	player[0].input = Input::KEYBOARD_ARROW;
	player[1].input = Input::KEYBOARD_WASD;
	player[2].input = Input::NONE;

	player[0].load("img/skin_diver.png");
	player[1].load("img/skin_blue.png");
	player[2].load("img/skin_cat.png");
	player[3].load("img/skin_bride.png");
/*
	Menu menu;
	menu.show();
	return 0;
*/
	game.play(4, 4);
	Screen::deinit();
	return 0;
}

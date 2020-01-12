#ifndef _BOMB_MENU_H
#define _BOMB_MENU_H

#include "def.h"
#include "sprite.h"

class Menu {
	Sprite background;
	bool input();

public:
	Menu();
	void stats();
	void show();
};

#endif

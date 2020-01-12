#pragma once

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

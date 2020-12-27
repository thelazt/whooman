#pragma once

#include "def.h"
#include "sprite.h"

class Menu {
	Sprite background, cursor, podium;

 public:
	Menu();
	void stats();
	void show();
};

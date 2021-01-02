#pragma once

#include "def.h"
#include "sprite.h"
#include "menuitem.h"

class Menu {
	Sprite background, cursor, podium, infoBox, infoBoxAni;

	MenuItemPlayerSkin * playerSkin[maxPlayer];
	MenuItemPlayerInput * playerInput[maxPlayer];

	MenuItemArenaSize * arenaSize;
	MenuItemArenaName * arenaName;
	MenuItemArenaLayout * arenaLayout;

	MenuItemSet * itemSet;

	MenuItemRound * round;

	MenuItemButton * buttonPlay;
	MenuItemButton * buttonInfo;
	MenuItemButton * buttonQuit;

 public:
	Menu();
	void show();
};

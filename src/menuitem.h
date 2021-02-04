#pragma once

#include "def.h"
#include "sprite.h"
#include "game.h"
#include "item.h"


class MenuItem {
 public:
	static MenuItem * active;
	static MenuItem * first;
	MenuItem * prev;
	MenuItem * next;

	static bool navBlocked;
	static bool pointBlocked;

	enum Mode {
		NORMAL = 0,
		ACTIVE = 1,
		PRESSED = 2,
		INACTIVE = 3,
	};

	enum Action {
		ACTION_UP = 0,
		ACTION_DOWN = 1,
		ACTION_NONE = 2
	};

	Action current_action;

 protected:
	MenuItem();
	virtual void navigationHelper(bool up, bool down, bool left, bool right, bool press) = 0;
	virtual bool pointerHelper(int x, int y, bool pressed) = 0;
	virtual void show() = 0;

 public:
	static void navigate(bool up, bool down, bool left, bool right, bool press);
	static bool pointer(int x, int y, bool pressed);
	static void showAll();
};

class MenuItemSelect : protected MenuItem {
	enum Mode buttonHelper(enum Action action);

 protected:
	static const unsigned short width = 46;
	static const unsigned short height = 22;
	static Sprite * button;
	static Sprite * label;

	const unsigned short x, yTop, yBottom;

	MenuItemSelect(unsigned short x, unsigned short y);

	virtual void handle(enum Action) = 0;
	virtual void draw() = 0;
	virtual bool isAllowed(enum Action action) {
		(void) action;
		return true;
	}

	void action(Action a, bool &block);
	void navigationHelper(bool up, bool down, bool left, bool right, bool press);
	bool pointerHelper(int x, int y, bool pressed);
	void show();
};

class MenuItemPlayerInput : protected MenuItemSelect {
	static Sprite * input;
	unsigned short p, px, py;

	void handle(enum Action action) override;
	void draw() override;

	Input::Method getDefault(unsigned short p);

 public:
	explicit MenuItemPlayerInput(unsigned short p);
};

class MenuItemPlayerSkin : protected MenuItemSelect {
	unsigned short p, px, py;

	static constexpr const char * sprites[] = {
		"img/skin_default.png",
		"img/skin_blue.png",
		"img/skin_brown.png",
		"img/skin_black.png",
		"img/skin_masked.png",
		"img/skin_diver.png",
		"img/skin_halloween.png",
		"img/skin_king.png",
		"img/skin_pig.png",
		"img/skin_pirate.png",
		"img/skin_santa.png",
		"img/skin_school.png",
		"img/skin_viking.png",
		"img/skin_work.png",
		"img/skin_bride.png",
		"img/skin_dog.png",
		"img/skin_mouse.png",
		"img/skin_cat.png",
		"img/skin_puma.png",
		"img/skin_tiger.png",
		"img/skin_dolphin.png",
		"img/skin_frog.png",
		"img/skin_croc.png",
		"img/skin_bear.png",
		"img/skin_bat.png",
	};

	static size_t skins[maxPlayer];

	void load(unsigned short p);
	void handle(enum Action action) override;
	void draw() override;

 public:
	explicit MenuItemPlayerSkin(unsigned short p);
};

class MenuItemArenaSize : protected MenuItemSelect {
	static Sprite * arenaSizes;
	int size;

	bool isAllowed(enum Action action) override;
	void handle(enum Action action) override;
	void draw() override;

 public:
	explicit MenuItemArenaSize(int size = 1);
	unsigned short getWidth();
	unsigned short getHeight();
};

class MenuItemArenaName : protected MenuItemSelect {
	static Sprite * arenaNames;
	int arena;

	void handle(enum Action action) override;
	void draw() override;

 public:
	explicit MenuItemArenaName(Game::ArenaName arena = Game::ARENA_CLASSIC);
	enum Game::ArenaName get();
};


class MenuItemArenaLayout : protected MenuItemSelect {
	static Sprite * arenaLayouts;
	int layout;

	void handle(enum Action action) override;
	void draw();

 public:
	explicit MenuItemArenaLayout(Game::LayoutName layout = Game::LAYOUT_CLASSIC);
	enum Game::LayoutName get();
};

class MenuItemSet: protected MenuItemSelect {
	static Sprite * itemsets;
	int itemset;

	void handle(enum Action action) override;
	void draw() override;

 public:
	explicit MenuItemSet(Item::ItemSet itemset = Item::SET_CLASSIC);
	enum Item::ItemSet get();
};


class MenuItemRound: protected MenuItemSelect {
	static Sprite * roundSprite;
	unsigned short rounds;

	bool isAllowed(enum Action action) override;
	void handle(enum Action action) override;
	void draw() override;

 public:
	explicit MenuItemRound(int rounds = 3);
	unsigned short get();
};


class MenuItemButton : protected MenuItem {
	static MenuItem * firstButton;
	static MenuItem * lastButton;


 protected:
	static const unsigned short width = 164;
	static const unsigned short height = 37;
	static Sprite * button;

	bool navDown, pointerDown;
	const unsigned short b, x, y;

	void switchButton(MenuItem * next);
	void navigationHelper(bool up, bool down, bool left, bool right, bool press);

	bool pointerHelper(int x, int y, bool pressed);
	void show();

 public:
	enum Button {
		BTN_PLAY = 0,
		BTN_INFO = 1,
		BTN_QUIT = 2,
	};

	explicit MenuItemButton(enum Button b);
	bool pressed(bool release = false);
};

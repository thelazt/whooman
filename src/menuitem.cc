#include "menuitem.h"

// Quick & ugly hacked menu...

#include "player.h"
#include "utils.h"
#include "input.h"
#include "menu.h"

MenuItem * MenuItem::active = nullptr;
MenuItem * MenuItem::first = nullptr;
bool MenuItem::navBlocked = false;
bool MenuItem::pointBlocked = false;

MenuItem::MenuItem() : prev(nullptr), next(nullptr), current_action(ACTION_NONE) {
	if (first == nullptr) {
		next = prev = active = first = this;
	} else {
		next = first;
		prev = first->prev;
		first->prev = prev->next = this;
	}
}

void MenuItem::navigate(bool up, bool down, bool left, bool right, bool press) {
	assert(active != nullptr);
	active->navigationHelper(up, down, left, right, press);
}

bool MenuItem::pointer(int x, int y, bool pressed) {
	MenuItem * i = first;
	do {
		assert(i != nullptr);
		if (i->pointerHelper(x, y, pressed))
			return true;
		i = i->next;
	} while (i != first);
	return false;
}

void MenuItem::showAll() {
	MenuItem * i = first;
	do {
		assert(i != nullptr);
		i->show();
		i = i->next;
	} while (i != first);
}



Sprite * MenuItemSelect::button = nullptr;
Sprite * MenuItemSelect::label = nullptr;

enum MenuItem::Mode MenuItemSelect::buttonHelper(enum Action action) {
	if (!isAllowed(action))
		return INACTIVE;
	else if (active == this)
		return current_action == action ? PRESSED : ACTIVE;
	else
		return NORMAL;
}

MenuItemSelect::MenuItemSelect(unsigned short x, unsigned short y) : x(x), yTop(y - height), yBottom(y + 80)  {
	if (button == nullptr)
		button = new Sprite("img/menu_select.png", width, height);
	if (label == nullptr)
		label = new Sprite("img/menu_label.png", 128, 25);
}



void MenuItemSelect::action(Action a, bool &block) {
	if (a == ACTION_NONE) {
		if (block) {
			block = false;
			current_action = ACTION_NONE;
		}
	} else if (!block && a != current_action) {
		if (a != ACTION_NONE) {
			if (active != this) {
				active->current_action = ACTION_NONE;
				active = this;
			}
			handle(a);
			block = true;
		}
		current_action = a;
	}
}

void MenuItemSelect::navigationHelper(bool up, bool down, bool left, bool right, bool press) {
	(void) press;
	if (up) {
		action(ACTION_UP, navBlocked);
	} else if (down) {
		action(ACTION_DOWN, navBlocked);
	} else if (left || right) {
		if (!navBlocked) {
			current_action = ACTION_NONE;
			active = left ? prev : next;
			active->current_action = ACTION_NONE;
			navBlocked = true;
		}
	} else {
		action(ACTION_NONE, navBlocked);
	}
}

bool MenuItemSelect::pointerHelper(int x, int y, bool pressed) {
	if (x >= this->x && x < this->x + width && y >= this->yTop && y < this->yBottom + height) {
		if (pressed && y >= this->yTop && y < this->yTop + height) {
			action(MenuItem::ACTION_UP, pointBlocked);
		} else if (pressed && y >= this->yBottom && y < this->yBottom + height) {
			action(MenuItem::ACTION_DOWN, pointBlocked);
		} else {
			action(MenuItem::ACTION_NONE, pointBlocked);
			pointBlocked = false;
		}
		return true;
	} else {
		return false;
	}
}

void MenuItemSelect::show() {
	draw();
	button->draw(ACTION_UP + 2 * buttonHelper(ACTION_UP), x, yTop);
	button->draw(ACTION_DOWN + 2 * buttonHelper(ACTION_DOWN), x, yBottom);
}



Sprite * MenuItemPlayerInput::input = nullptr;

void MenuItemPlayerInput::handle(enum Action action) {
	while (true) {
		int i = static_cast<int>(player[p].input);
		if (action == ACTION_UP) {
			if (--i < 0)
				i = Input::NONE;
		} else if (action == ACTION_DOWN) {
			if (++i > Input::NONE)
				i = 0;
		}
		player[p].input = static_cast<Input::Method>(i);

		int controls = 0;
		for (unsigned i = 0; i < 4; i++)
			if (player[p].input == player[i].input)
				controls++;

		// At least 2 Players
		if (player[p].input == Input::NONE && controls <= 2)
			return;
		// AI always okay
		else if (player[p].input == Input::AI)
			return;
		// Each Input device can only be used once
		else if (controls == 1)
			return;
	}
}

void MenuItemPlayerInput::draw() {
	input->draw(player[p].input, px, py);
}

Input::Method MenuItemPlayerInput::getDefault(unsigned short p) {
	switch (p) {
		case 0: return Input::KEYBOARD_ARROW;
		case 1: return Input::KEYBOARD_WASD;
		case 2: return Input::AI;
		default: return Input::NONE;
	}
}


MenuItemPlayerInput::MenuItemPlayerInput(unsigned short p)
  : MenuItemSelect(221 + p * 200, 375), p(p), px(170 + p * 200), py(380) {
	if (input == nullptr) {
		input = new Sprite("img/menu_control.png", 96, 72);
	}
	player[p].input = getDefault(p);
}



size_t MenuItemPlayerSkin::skins[maxPlayer];
constexpr const char * MenuItemPlayerSkin::sprites[];

void MenuItemPlayerSkin::load(unsigned short p) {
	assert(p < size(skins));
	assert(skins[p] < size(sprites));
	player[p].load(sprites[skins[p]]);
}

void MenuItemPlayerSkin::handle(enum Action action) {
	bool unique;
	do {
		if (action == ACTION_UP) {
			if (skins[p]-- == 0)
				skins[p] = size(sprites) - 1;
		} else if (action == ACTION_DOWN) {
			if (++skins[p] >= static_cast<int>(size(sprites)))
				skins[p] = 0;
		}

		unique = true;
		for (unsigned i = 0; i < 4; i++)
			if (p != i && skins[p] == skins[i]) {
				unique = false;
				break;
			}
	} while (!unique);
	load(p);
}

void MenuItemPlayerSkin::draw() {
	label->draw(p + (active == this || active == next ? 7 : 0), px - 2, py - 55);
	player[p].skin->draw(1, px, py + 5);
}

MenuItemPlayerSkin::MenuItemPlayerSkin(unsigned short p)
  : MenuItemSelect(157 + p * 200, 375), p(p), px(148 + p * 200), py(375) {
	assert(p < size(skins));
	skins[p] = p;
	load(p);
}



Sprite * MenuItemArenaSize::arenaSizes = nullptr;

bool MenuItemArenaSize::isAllowed(enum Action action) {
	return (action == ACTION_UP && size > 0) || (action == ACTION_DOWN && size < 2);
}

void MenuItemArenaSize::handle(enum Action action) {
	if (isAllowed(action)) {
		if (action == ACTION_UP)
			size--;
		else if (action == ACTION_DOWN)
			size++;
	}
}

void MenuItemArenaSize::draw() {
	arenaSizes->draw(size, 148, 664);
}

MenuItemArenaSize::MenuItemArenaSize(int size) : MenuItemSelect(157, 605), size(size) {
	assert(size > 0 && size < 2);
	if (arenaSizes == nullptr) {
		arenaSizes = new Sprite("img/menu_size.png", 80, 16);
	}
}

unsigned short MenuItemArenaSize::getWidth() {
	switch (size) {
		case 0: return   9;
		case 2: return  15;
		default: return 13;
	}
}

unsigned short MenuItemArenaSize::getHeight() {
	switch (size) {
		case 0: return  11;
		case 2: return  19;
		default: return 15;
	}
}



Sprite * MenuItemArenaName::arenaNames = nullptr;

void MenuItemArenaName::handle(enum Action action) {
	if (action == ACTION_UP) {
		if (--arena < 0)
			arena = Game::ARENA_RANDOM;
	} else if (action == ACTION_DOWN) {
		if (++arena > Game::ARENA_RANDOM)
			arena = 0;
	}
}

void MenuItemArenaName::draw() {
	label->draw(active == prev || active == this || active == next ? 11 : 4, 165, 550);
	arenaNames->draw(arena, 185, 607);
}

MenuItemArenaName::MenuItemArenaName(Game::ArenaName arena) : MenuItemSelect(205, 605), arena(arena) {
	if (arenaNames == nullptr) {
		arenaNames = new Sprite("img/menu_playgrounds.png", 96, 59);
	}
}

enum Game::ArenaName MenuItemArenaName::get() {
	return static_cast<enum Game::ArenaName>(arena);
}



Sprite * MenuItemArenaLayout::arenaLayouts = nullptr;

void MenuItemArenaLayout::handle(enum Action action) {
	if (action == ACTION_UP) {
		if (--layout < 0)
			layout = Game::LAYOUT_RANDOM;
	} else if (action == ACTION_DOWN) {
		if (++layout > Game::LAYOUT_RANDOM)
			layout = 0;
	}
}

void MenuItemArenaLayout::draw() {
	arenaLayouts->draw(layout, 232, 664);
}

MenuItemArenaLayout::MenuItemArenaLayout(Game::LayoutName layout) : MenuItemSelect(257, 605), layout(layout) {
	if (arenaLayouts == nullptr) {
		arenaLayouts = new Sprite("img/menu_layout.png", 104, 16);
	}
}

enum Game::LayoutName MenuItemArenaLayout::get() {
	return static_cast<enum Game::LayoutName>(layout);
}



Sprite * MenuItemSet::itemsets = nullptr;

void MenuItemSet::handle(enum Action action) {
	if (action == ACTION_UP) {
		if (--itemset < 0)
			itemset = Item::SET_RANDOM;
	} else if (action == ACTION_DOWN) {
		if (++itemset > Item::SET_RANDOM)
			itemset = 0;
	}
}

void MenuItemSet::draw() {
	label->draw(active == this ? 13 : 6, 365, 550);
	itemsets->draw(itemset, 390, 608);
}

MenuItemSet::MenuItemSet(Item::ItemSet itemset) : MenuItemSelect(400, 605), itemset(itemset) {
	if (itemsets == nullptr) {
		itemsets = new Sprite("img/menu_items.png", 74, 74);
	}
}

enum Item::ItemSet MenuItemSet::get() {
	return static_cast<enum Item::ItemSet>(itemset);
}



Sprite * MenuItemRound::roundSprite = nullptr;

bool MenuItemRound::isAllowed(enum Action action) {
	return (action == ACTION_UP && rounds > 3) ||  (action == ACTION_DOWN && rounds < 7);
}

void MenuItemRound::handle(enum Action action) {
	if (isAllowed(action)) {
		if (action == ACTION_UP)
			rounds -= 2;
		else if (action == ACTION_DOWN)
			rounds += 2;
	}
}

void MenuItemRound::draw() {
	label->draw(active == this ? 12 : 5, 535, 550);
	roundSprite->draw((rounds - 3) / 2, 572, 615);
}

MenuItemRound::MenuItemRound(int rounds) : MenuItemSelect(570, 605), rounds(rounds) {
	assert(rounds == 3 || rounds == 5 || rounds == 7);
	if (roundSprite == nullptr) {
		roundSprite = new Sprite("img/menu_round.png", 52, 60);
	}
}

unsigned short MenuItemRound::get() {
	return rounds;
}



Sprite * MenuItemButton::button = nullptr;
MenuItem * MenuItemButton::firstButton = nullptr;
MenuItem * MenuItemButton::lastButton = nullptr;

/*bool MenuItemButton::navDown = false;
bool MenuItemButton::pointerDown = false;
*/
MenuItemButton::MenuItemButton(enum Button b) : navDown(false), pointerDown(false), b(b), x(703), y(580 + b * 40) {
	if (button == nullptr)
		button = new Sprite("img/menu_button.png", width, height);
	if (firstButton == nullptr)
		firstButton = this;
	lastButton = this;
}

void MenuItemButton::switchButton(MenuItem * next) {
	if (!navBlocked) {
		current_action = ACTION_NONE;
		active = next;
		active->current_action = ACTION_NONE;
		navBlocked = true;
		navDown = false;
	}
}

void MenuItemButton::navigationHelper(bool up, bool down, bool left, bool right, bool press) {
	if (up) {
		if (firstButton != this)
			switchButton(prev);
	} else if (down) {
		if (lastButton != this)
			switchButton(next);
	} else if (left || right) {
		switchButton(left ? firstButton->prev : lastButton->next);
	} else if (press) {
		if (!navBlocked) {
			navBlocked = true;
			navDown = true;
		}
	} else {
		if (navBlocked) {
			navBlocked = false;
			navDown = false;
			current_action = ACTION_NONE;
		}
	}
}

bool MenuItemButton::pointerHelper(int x, int y, bool pressed) {
	if (x >= this->x && x < this->x + width && y >= this->y && y < this->y + height) {
		if (pressed) {
			if (active != this) {
				current_action = ACTION_NONE;
				active = this;
			}
			if (!pointBlocked) {
				pointerDown = true;
				pointBlocked = true;
			}
		} else {
			pointBlocked = false;
			pointerDown = false;
		}
		return true;
	} else {
		pointBlocked = false;
		pointerDown = false;
		return false;
	}
}

void MenuItemButton::show() {
	button->draw(b + 3 * (active == this ? (navDown || pointerDown ? PRESSED : ACTIVE) : NORMAL), x, y);
}

bool MenuItemButton::pressed(bool release) {
	if (active == this && (navDown || pointerDown)) {
		if (release) {
			navDown = false;
			pointerDown = false;
		}
		return true;
	} else {
		navDown = false;
		pointerDown = false;
		return false;
	}
}

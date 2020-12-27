#include "menu.h"

#include "player.h"
#include "utils.h"
#include "input.h"
#include "def.h"

class MenuItem {
 public:
	static const unsigned short width = 46;
	static const unsigned short height = 22;
	static MenuItem * active;
	static MenuItem * first;
	static Sprite * button;
	static Sprite * label;
	MenuItem * prev;
	MenuItem * next;

 protected:
	enum Action {
		ACTION_UP = 0,
		ACTION_DOWN = 1,
		ACTION_NONE = 2
	};
	enum Mode {
		NORMAL = 0,
		ACTIVE = 2,
		PRESSED = 4,
		INACTIVE = 6,
	};
	const unsigned short x, yTop, yBottom;
	Action current_action;

	MenuItem(unsigned short x, unsigned short y, unsigned short w, unsigned short h) : prev(nullptr), next(nullptr),
	    x(x + (w - width) / 2), yTop(y - height), yBottom(y + h), current_action(ACTION_NONE)  {
		if (button == nullptr)
			button = new Sprite("img/menu_select.png", 46, 22);
		if (label == nullptr)
			label = new Sprite("img/menu_label.png", 128, 25);
	}

	virtual void handle(enum Action) = 0;
	virtual void draw() = 0;

 private:
	static bool navBlocked;
	static bool pointBlocked;

	void action(Action a, bool &block) {
		if (a == ACTION_NONE) {
			if (block && active == this) {
				block = false;
				current_action = ACTION_NONE;
			} else if (!block && active != this) {
				active->current_action = ACTION_NONE;
				active = this;
				block = true;
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
	bool pointerHelper(int x, int y, bool pressed) {
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

	void show() {
		draw();
		button->draw((active == this ? (current_action == ACTION_UP ? PRESSED : ACTIVE) : NORMAL) + ACTION_UP, x, yTop);
		button->draw((active == this ? (current_action == ACTION_DOWN ? PRESSED : ACTIVE) : NORMAL) + ACTION_DOWN, x, yBottom);
	}

 public:
	static void navigate(bool up, bool down, bool left, bool right) {
		assert(active != nullptr);
		if (up)
			active->action(ACTION_UP, navBlocked);
		else if (down)
			active->action(ACTION_DOWN, navBlocked);
		else if (left)
			active->prev->action(ACTION_NONE, navBlocked);
		else if (right)
			active->next->action(ACTION_NONE, navBlocked);
		else
			active->action(ACTION_NONE, navBlocked);
	}

	static bool pointer(int x, int y, bool pressed) {
		MenuItem * i = first;
		do {
			assert(i != nullptr);
			if (i->pointerHelper(x, y, pressed))
				return true;
			i = i->next;
		} while (i != first);
		return false;
	}

	static void showAll() {
		MenuItem * i = first;
		do {
			assert(i != nullptr);
			i->show();
			i = i->next;
		} while (i != first);
	}
};

class MenuItemPlayerInput : public MenuItem {
	static Sprite * input;
	unsigned short p, px, py;

	void handle(enum Action action) {
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

	void draw() {
		input->draw(player[p].input, px, py);
	}

	Input::Method getDefault(unsigned short p) {
		switch (p) {
			case 0: return Input::KEYBOARD_ARROW;
			case 1: return Input::KEYBOARD_WASD;
			case 2: return Input::AI;
			default: return Input::NONE;
		}
	}

 public:
	explicit MenuItemPlayerInput(unsigned short p)
	  : MenuItem(182 + p * 200, 355, 64, 80), p(p), px(140 + p * 200), py(360) {
		if (input == nullptr) {
			input = new Sprite("img/menu_control.png", 96, 72);
		}
		player[p].input = getDefault(p);
	}
};

class MenuItemPlayerSkin : public MenuItem {
	const MenuItemPlayerInput * input;
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

	void load(unsigned short p) {
		assert(p < size(skins));
		assert(skins[p] < size(sprites));
		player[p].load(sprites[skins[p]]);
	}

	void handle(enum Action action) {
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

	void draw() {
		label->draw(p + (active == this || active == input ? 8 : 0), px, py - 55);
		player[p].skin->draw(1, px, py + 5);
	}

 public:
	MenuItemPlayerSkin(unsigned short p, MenuItemPlayerInput * i)
	  : MenuItem(118 + p * 200, 355, 64, 80), input(i), p(p), px(118 + p * 200), py(355) {
		assert(p < size(skins));
		skins[p] = p;
		load(p);
	}
};


Sprite * MenuItem::button = nullptr;
Sprite * MenuItem::label = nullptr;

MenuItem * MenuItem::active = nullptr;
MenuItem * MenuItem::first = nullptr;
bool MenuItem::navBlocked = false;
bool MenuItem::pointBlocked = false;
Sprite * MenuItemPlayerInput::input = nullptr;
size_t MenuItemPlayerSkin::skins[maxPlayer];
constexpr const char * MenuItemPlayerSkin::sprites[];

Menu::Menu()
  : background("img/menu_background.png", 1024, 768),
    cursor("img/menu_cursor.png", 17, 24),
    podium("img/menu_podium.png", 192, 122) {

	MenuItem * prev = nullptr;
	// Player setup
	for (unsigned short p = 0; p < maxPlayer; p++) {
		MenuItemPlayerInput * i = new MenuItemPlayerInput(p);
		MenuItemPlayerSkin * s = new MenuItemPlayerSkin(p, i);
		if (MenuItem::first == nullptr)
			MenuItem::first = s;
		if (prev != nullptr)
			prev->next = s;
		s->prev = prev;
		s->next = i;
		i->prev = s;
		prev = i;
	}
	MenuItem::first->prev = prev;
	prev->next = MenuItem::first;
	MenuItem::active = MenuItem::first;
}


void Menu::stats() {
	const unsigned short wleft = 416;
	bool ani = false;
	while (Input::update()) {
		ani = !ani;
		Screen::lock();
		background.draw(0, 0, 0);
		podium.draw(0, wleft, 400);
		player[0].skin->draw(1, wleft + 8, 390);
		player[2].skin->draw(12, wleft + 118, 400);
		podium.draw(1, wleft, 400);
		player[1].skin->draw(ani ? 15 : 16, wleft + 62, 360);
		Screen::unlock();
		Screen::flip();
		GuardedBell::sleep(300);
	}
}

void Menu::show() {
	while (Input::update()) {
		// Keyboard
		MenuItem::navigate(Input::active(Input::KEYBOARD_ARROW, Input::MOVE_UP),
		                   Input::active(Input::KEYBOARD_ARROW, Input::MOVE_DOWN),
						   Input::active(Input::KEYBOARD_ARROW, Input::MOVE_LEFT),
						   Input::active(Input::KEYBOARD_ARROW, Input::MOVE_RIGHT));

		// Mouse
		int mouse_x, mouse_y;
		bool mouse_button = Input::mouse(mouse_x, mouse_y);
		bool mouse_hover = MenuItem::pointer(mouse_x, mouse_y, mouse_button);

		// Draw
		Screen::lock();

		background.draw(0, 0, 0);

		MenuItem::showAll();

		cursor.draw((mouse_hover ? (mouse_button ? 1 : 0) : 2), mouse_x, mouse_y);

		Screen::unlock();
		Screen::flip();
		GuardedBell::sleep(1);
	}

}

#include "menu.h"

#include "player.h"
#include "utils.h"
#include "input.h"

const char * skin[] = {
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

class Select {
	static Sprite * button;
	static unsigned short active;
	static const unsigned short width = 46;
	static const unsigned short height = 22;
	enum {
		TOP = 0,
		BOTTOM = 1,
	};
	enum Mode {
		NORMAL = 0,
		ACTIVE = 2,
		PRESSED = 4,
		INACTIVE = 6,
	};
	const unsigned short idx, x, yTop, yBottom;

 public:
	Select(unsigned short idx, unsigned short x, unsigned short y, unsigned short w, unsigned short h)
	  : idx(idx), x(x + (w - width) / 2), yTop(y - height), yBottom(y + h) {
		if (button == nullptr) {
			button = new Sprite("img/menu_select.png", 46, 22);
		}
	}

	void draw() {
		enum Mode mode = active == idx ? ACTIVE : NORMAL;
		button->draw(mode + TOP, x, yTop);
		button->draw(mode + BOTTOM, x, yBottom);
	}
};
Sprite * Select::button = nullptr;
unsigned short Select::active = 0;


Menu::Menu() : background("img/menu_background.png", 1024, 768) {
	player[0].input = Input::KEYBOARD_ARROW;
	player[1].input = Input::KEYBOARD_WASD;
	player[2].input = Input::NONE;

	for (unsigned p = 0; p < 4; p++) {
		player[p].load(skin[p]);
	}
}


void Menu::stats() {
	Sprite podium("img/menu_podium.png", 192, 122);

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
	Sprite label("img/menu_label.png", 128, 25);
	Sprite control("img/menu_control.png", 96, 72);
	Sprite playground("img/menu_playgrounds.png", 96, 59);

	const unsigned short labelTop = 300;

	Select selPlayerSkin[maxPlayer] = {
		{0, 118, 355, 64, 80},
		{2, 318, 355, 64, 80},
		{4, 518, 355, 64, 80},
		{6, 718, 355, 64, 80}
	};

	Select selPlayerControl[maxPlayer] = {
		{1, 182, 355, 64, 80},
		{3, 382, 355, 64, 80},
		{5, 582, 355, 64, 80},
		{7, 782, 355, 64, 80}
	};

	while (Input::update()) {
		Screen::lock();
		background.draw(0, 0, 0);
		for (unsigned short p = 0; p < maxPlayer; p++) {
			label.draw(p, 118 + p * 200, labelTop);
			player[p].skin->draw(1, 118 + p * 200, labelTop + 60);
			control.draw(p+2, 140 + p * 200,  labelTop + 60);
			selPlayerSkin[p].draw();
			selPlayerControl[p].draw();
		}
		Screen::unlock();
		Screen::flip();
		GuardedBell::sleep(1);
	}

}

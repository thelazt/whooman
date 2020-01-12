#include "menu.h"

#include "player.h"
#include "utils.h"
#include "input.h"

static const char * skins[] = {
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

Menu::Menu() : background("img/menu_background.png", 1024, 768) {}


void Menu::stats() {
	Sprite podium("img/menu_podium.png", 192, 122);

	const unsigned short wleft = 416;
	bool ani = false;
	while (Input::update()) {
		ani = !ani;
		Screen::lock();
		background.draw(0, 0, 0);
		podium.draw(0, wleft, 400);
		player[0].skin.draw(1, wleft + 8, 390);
		player[2].skin.draw(12, wleft + 118, 400);
		podium.draw(1, wleft, 400);
		player[1].skin.draw(ani ? 15 : 16, wleft + 62, 360);
		Screen::unlock();
		Screen::flip();
		usleep(300000);
	}
}

void Menu::show() {
	Sprite label("img/menu_label.png", 128, 25);
	Sprite control("img/menu_control.png", 96, 72);
	Sprite playground("img/menu_playgrounds.png", 96, 59);

	while (Input::update()) {
		Screen::lock();
		background.draw(0, 0, 0);
		const unsigned short labelTop = 300;
		for (unsigned short p = 0; p < maxPlayer; p++) {
			label.draw(p, 112 + p * 200, labelTop);
			player[p].skin.draw(1, 112 + p * 200, labelTop + 40);
			control.draw(p+2, 146 + p * 200,  labelTop + 40);
		}
		Screen::unlock();
		Screen::flip();
		wait(20);
	}

}

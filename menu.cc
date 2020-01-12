#include "SDL/SDL.h"
#include <unistd.h>

#include "menu.h"

#include "player.h"

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

bool Menu::input(){
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			return false;
/*		else
			for (unsigned short p = 0; p < maxPlayer; p++)
				for (enum Player::PlayerDir e = Player::MOVE_DOWN; e <= Player::MOVE_BOMB; e = (enum Player::PlayerDir)(e + 1))
					if (event.key.keysym.sym == player[p].keys[e]){
						if (event.type == SDL_KEYDOWN){
							if (e == Player::MOVE_BOMB)
								player[p].bomb();
							else
								move[p] = e;
						} else if (event.type == SDL_KEYUP && move[p] == e)
							move[p] = Player::MOVE_WAIT;
					}
*/
	}
	return true;
}


void Menu::stats(){
	Sprite podium("img/menu_podium.png", 192, 122);

	const unsigned short wleft = 416;
	bool ani = false;
	while (input()){
		ani = !ani;
		screen.lock();
		background.draw(0,0,0);
		podium.draw(0, wleft,400);
		player[0].skin.draw(1, wleft + 8, 390);
		player[2].skin.draw(12, wleft + 118, 400);
		podium.draw(1, wleft,400);
		player[1].skin.draw(ani ? 15 : 16, wleft + 62, 360);
		screen.unlock();
		screen.flip();
		usleep(300000);
	}
}

void Menu::show(){
	Sprite label("img/menu_label.png", 128, 25);
	Sprite control("img/menu_control.png", 96, 72);
	Sprite playground("img/menu_playgrounds.png", 96, 59);

	while (input()){
		screen.lock();
		background.draw(0,0,0);
		const unsigned short labelTop = 300;
		for (unsigned short p = 0; p < maxPlayer; p++){
			label.draw(p, 112 + p * 200, labelTop);
			player[p].skin.draw(1, 112 + p * 200, labelTop + 40);
			control.draw(p+2, 146 + p * 200,  labelTop + 40);
		}
		screen.unlock();
		screen.flip();
		usleep(300000);
	}

}

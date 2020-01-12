#include <iostream>
#include <stdint.h>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <ctime>


#include "SDL/SDL.h"

#include "screen.h"
#include "playground.h"
#include "layout.h"
#include "player.h"
#include "game.h"


using namespace std;

Screen screen(1024,768);

int number(){
	return rand();
}


int main(){
	srand(time(NULL));

	SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL);

	player[0].keys[Player::MOVE_UP] = SDLK_UP;
	player[0].keys[Player::MOVE_DOWN] = SDLK_DOWN;
	player[0].keys[Player::MOVE_LEFT] = SDLK_LEFT;
	player[0].keys[Player::MOVE_RIGHT] = SDLK_RIGHT;
	player[0].keys[Player::MOVE_BOMB] = SDLK_SPACE;

	player[1].keys[Player::MOVE_UP] = SDLK_w;
	player[1].keys[Player::MOVE_DOWN] = SDLK_s;
	player[1].keys[Player::MOVE_LEFT] = SDLK_a;
	player[1].keys[Player::MOVE_RIGHT] = SDLK_d;
	player[1].keys[Player::MOVE_BOMB] = SDLK_LSHIFT;

	player[2].keys[Player::MOVE_UP] = SDLK_i;
	player[2].keys[Player::MOVE_DOWN] = SDLK_k;
	player[2].keys[Player::MOVE_LEFT] = SDLK_j;
	player[2].keys[Player::MOVE_RIGHT] = SDLK_l;
	player[2].keys[Player::MOVE_BOMB] = SDLK_RSHIFT;

	player[0].load("img/skin_diver.png");
	player[1].load("img/skin_blue.png");
	player[2].load("img/skin_cat.png");
	player[3].load("img/skin_bride.png");

	return game.play(3,4) ? 0 : 1;
}

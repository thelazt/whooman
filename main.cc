#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <ctime>
#include <random>


#include "SDL/SDL.h"

#include "screen.h"
#include "playground.h"
#include "layout.h"
#include "arena.h"
#include "player.h"


using namespace std;


Arena arena_default(10,10);
Layout layout_default;

SDL_Event event;

Screen screen(1024,768);

int number(){
	return std::rand();
}

int main(){
	std::srand(std::time(nullptr));

	SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL);

	player[0].load("img/skin_diver.png");
	player[1].load("img/skin_blue.png");
	player[2].load("img/skin_cat.png");
	player[3].load("img/skin_bride.png");

	if (!playground.create(arena_default, layout_default)){
		cout << "Could not create Playground..."  << endl;
		return 1;
	}

	int quit = 0;
	unsigned int i = 0;
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						player[0].move(Player::MOVE_UP);
						break;
					case SDLK_DOWN:
						player[0].move(Player::MOVE_DOWN);
						break;
					case SDLK_LEFT:
						player[0].move(Player::MOVE_LEFT);
						break;
					case SDLK_RIGHT:
						player[0].move(Player::MOVE_RIGHT);
						break;
					case SDLK_SPACE:
						player[0].bomb();
						break;
					case SDLK_d:
						playground.dump();
						break;
					case SDLK_ESCAPE:
						quit = 1;
						break;
					default:
						break;
				}
			}
			else if (event.type == SDL_QUIT) {
				quit = 1;
			}
		}
		for (int p = 1; p < playground.playerCount(); p++)
			player[p].move(Player::MOVE_AUTO);
		if (i++ % 16 == 0)
			playground.tick();
		playground.draw();
		usleep(5000);
	}

	return 0;
}

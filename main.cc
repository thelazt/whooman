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
#include "arena_default.h"
#include "player.h"


using namespace std;

ArenaFactory arena_default(100,48);
Layout layout_default;

SDL_Event event;

Screen screen(1024,768);

int number(){
	return rand();
}

int main(){
	srand(time(NULL));
	//playground.dim(10,10);
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
	enum Player::PlayerDir move[4];
	move[0] = move[3] = Player::MOVE_WAIT;
	move[1] = move[2] = Player::MOVE_AUTO;
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			switch (event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_UP:
							move[3] = Player::MOVE_UP;
							break;
						case SDLK_DOWN:
							move[3] = Player::MOVE_DOWN;
							break;
						case SDLK_LEFT:
							move[3] = Player::MOVE_LEFT;
							break;
						case SDLK_RIGHT:
							move[3] = Player::MOVE_RIGHT;
							break;
						case SDLK_SPACE:
							player[3].bomb();
							break;

						case SDLK_w:
							move[0] = Player::MOVE_UP;
							break;
						case SDLK_s:
							move[0] = Player::MOVE_DOWN;
							break;
						case SDLK_a:
							move[0] = Player::MOVE_LEFT;
							break;
						case SDLK_d:
							move[0] = Player::MOVE_RIGHT;
							break;
						case SDLK_LSHIFT:
							player[0].bomb();
							break;

						case SDLK_ESCAPE:
							quit = 1;
							break;
						default:
							break;
					}
					break;

				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_UP:
							if (move[3] == Player::MOVE_UP)
								move[3] = Player::MOVE_WAIT;
							break;
						case SDLK_DOWN:
							if (move[3] == Player::MOVE_DOWN)
								move[3] = Player::MOVE_WAIT;
							break;
						case SDLK_LEFT:
							if (move[3] == Player::MOVE_LEFT)
								move[3] = Player::MOVE_WAIT;
							break;
						case SDLK_RIGHT:
							if (move[3] == Player::MOVE_RIGHT)
								move[3] = Player::MOVE_WAIT;
							break;

						case SDLK_w:
							if (move[0] == Player::MOVE_UP)
								move[0] = Player::MOVE_WAIT;
							break;
						case SDLK_s:
							if (move[0] == Player::MOVE_DOWN)
								move[0] = Player::MOVE_WAIT;
							break;
						case SDLK_a:
							if (move[0] == Player::MOVE_LEFT)
								move[0] = Player::MOVE_WAIT;
							break;
						case SDLK_d:
							if (move[0] == Player::MOVE_RIGHT)
								move[0] = Player::MOVE_WAIT;
							break;

						default:
							break;
					}
					break;

				case SDL_QUIT:
					quit = 1;
					break;
			}
		}
		for (int p = 0; p < playground.playerCount(); p++)
			player[p].move(move[p]);
		if (i++ % 4 == 0)
			playground.tick();
		playground.draw();
		usleep(10000);
	}

	return 0;
}

#include "input.h"

#include "SDL/SDL.h"

namespace Input {

static const int keyMethods = 3;
static const int keys[keyMethods][ACTIONS] = {
	{
		SDLK_DOWN,
		SDLK_LEFT,
		SDLK_RIGHT,
		SDLK_UP,
		SDLK_RCTRL,
	}, {
		SDLK_s,
		SDLK_a,
		SDLK_d,
		SDLK_w,
		SDLK_LSHIFT,
	}, {
		SDLK_j,
		SDLK_h,
		SDLK_k,
		SDLK_u,
		SDLK_b,
	}
};

static bool state[METHODS][ACTIONS];
static SDL_Event event;

bool update() {
	static bool init = false;
	if (!init) {
		init = true;
		SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL);
		for (unsigned short m = 0; m < METHODS; m++)
			for (int a = 0; a < ACTIONS; a++)
				state[m][a] = false;
	}

	while(SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			return false;
		} else {
			for (unsigned short k = 0; k < keyMethods; k++) {
				for (int a = 0; a < ACTIONS; a++) {
					if (event.key.keysym.sym == keys[k][a]) {
						if (event.type == SDL_KEYDOWN)
							state[k][a] = true;
						else if (event.type == SDL_KEYUP)
							state[k][a] = false;
					}
				}
			}
		}
	}
	return true;
}

bool active(enum Method method, enum Action action) {
	return state[method][action];
}
}  // namespace Input

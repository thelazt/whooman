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

static bool state[AI][ACTIONS];
static SDL_Event event;
static int x, y;
static const int threshold = 2;

bool update() {
	static bool init = false;
	if (!init) {
		init = true;
		SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL);
		for (unsigned short m = 0; m < NONE; m++)
			for (int a = 0; a < ACTIONS; a++)
				state[m][a] = false;
	}

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return false;
			case SDL_KEYUP:
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
				break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			  {
				int oldx = x;
				int oldy = y;
				state[Method::MOUSE][Action::PLACE_BOMB] = SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT);
				state[Method::MOUSE][Action::MOVE_LEFT] = x < oldx + threshold;
				state[Method::MOUSE][Action::MOVE_RIGHT] = x > oldx - threshold;
				state[Method::MOUSE][Action::MOVE_UP] = y < oldy + threshold;
				state[Method::MOUSE][Action::MOVE_DOWN] = y > oldy - threshold;
				break;
			  }
		}
	}
	return true;
}

bool active(enum Method method, enum Action action) {
	return state[method][action];
}

bool mouse(int &pos_x, int &pos_y) {
	pos_x = x;
	pos_y = y;
	return state[Method::MOUSE][Action::PLACE_BOMB];
}
}  // namespace Input

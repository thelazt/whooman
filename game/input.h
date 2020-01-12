#pragma once

namespace Input {

enum Action {
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	PLACE_BOMB,

	ACTIONS
};

enum Method {
	KEYBOARD_ARROW,
	KEYBOARD_WASD,
	KEYBOARD_UHJK,

	MOUSE,

	METHODS,

	NONE,
};

bool update();

bool active(enum Method method, enum Action action);

}  // namespace Input

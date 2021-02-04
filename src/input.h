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

	AI,

	NONE,
};

enum Control {
	CONTROL_NONE,
	CONTROL_ENTER,
	CONTROL_EXIT
};

enum Control update();

bool active(enum Method method, enum Action action);

bool mouse(unsigned &pos_x, unsigned &pos_y);

}  // namespace Input

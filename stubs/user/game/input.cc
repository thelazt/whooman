#include "input.h"

#include "object/key.h"
#include "machine/ps2controller.h"
#include "device/mouse.h"
#include "object/pointer.h"
extern Mouse mouseDev;

namespace Input {

static const int keyMethods = 3;
static const Key::Scancode keys[keyMethods][ACTIONS] = {
	{
		Key::Scancode::KEY_DOWN,
		Key::Scancode::KEY_LEFT,
		Key::Scancode::KEY_RIGHT,
		Key::Scancode::KEY_UP,
		Key::Scancode::KEY_LEFT_CTRL,
	}, {
		Key::Scancode::KEY_S,
		Key::Scancode::KEY_A,
		Key::Scancode::KEY_D,
		Key::Scancode::KEY_W,
		Key::Scancode::KEY_LEFT_SHIFT,
	}, {
		Key::Scancode::KEY_J,
		Key::Scancode::KEY_H,
		Key::Scancode::KEY_K,
		Key::Scancode::KEY_U,
		Key::Scancode::KEY_B,
	}
};

static bool mouseState[ACTIONS];
static const int threshold = 3;

enum Control update() {

	int x, y;
	mouseState[Action::PLACE_BOMB] = mouseDev.delta(x, y);
	int deltaX = x < 0 ? - x : x;
	int deltaY = y < 0 ? - y : y;
	mouseState[Action::MOVE_LEFT] = false;
	mouseState[Action::MOVE_RIGHT] = false;
	mouseState[Action::MOVE_UP] = false;
	mouseState[Action::MOVE_DOWN] = false;
	if (deltaX > deltaY) {
		mouseState[Action::MOVE_LEFT] = x < -threshold;
		mouseState[Action::MOVE_RIGHT] = x > threshold;
	} else {
		mouseState[Action::MOVE_UP] = y > threshold;
		mouseState[Action::MOVE_DOWN] = y < -threshold;
	}

	if (PS2Controller::isPressed(Key::Scancode::KEY_ESCAPE))
		return Input::CONTROL_EXIT;
	else if (PS2Controller::isPressed(Key::Scancode::KEY_ENTER))
		return Input::CONTROL_ENTER;
	else
		return Input::CONTROL_NONE;
}

bool active(enum Method method, enum Action action) {
	switch (method) {
		case KEYBOARD_ARROW:
		case KEYBOARD_WASD:
		case KEYBOARD_UHJK:
			return PS2Controller::isPressed(keys[method][action]);

		case MOUSE:
			return mouseState[action];

		default:
			return false;
	}
}

bool mouse(unsigned &pos_x, unsigned &pos_y) {
	mouseDev.getPos(pos_x, pos_y);
	return mouseDev.isPressed(MOUSE_LEFT);
}

}  // namespace Input

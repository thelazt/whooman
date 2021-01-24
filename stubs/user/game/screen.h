#pragma once

#include "syscall/guarded_graphics.h"
#include "interrupt/guarded.h"

extern GuardedGraphics graphics;

namespace Screen {
	inline unsigned int getWidth() {
		return graphics.width();
	}

	inline unsigned int getHeight() {
		return graphics.height();
	}

	inline void lock() {}

	inline void unlock() {}

	inline void flip(bool duplicate = false) {
		graphics.switchBuffers(duplicate);
	}
}  // namespace Screen

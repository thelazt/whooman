#pragma once

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <unistd.h>

#include "def.h"

using std::endl;

#define DBG std::cerr
#define USHRT_MAX 65535

inline void wait(int ticks = 1) {
	usleep(1000 * static_cast<int>(SUBTICK_MS) * ticks);
}

inline int number() {
	return rand();
}

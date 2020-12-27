#pragma once

#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <iostream>

#include "def.h"

using std::endl;

#define DBG std::cerr
#define USHRT_MAX 65535

class GuardedBell {
	int t;

 public:
	void set(int ticks) {
		t = ticks;
	}

	void sleep() {
		usleep(1000 * t);
	}

	static void sleep(unsigned int ms) {
		GuardedBell bell;
		bell.set(ms);
		bell.sleep();
	}
};

inline unsigned number() {
	return rand();
}

template<class T, size_t N>
constexpr size_t size(T (&/*unused*/)[N]) {
	return N;
}

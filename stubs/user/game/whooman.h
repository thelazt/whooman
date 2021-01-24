#pragma once

#include "thread/thread.h"

const size_t STACK_SIZE = 8388608;

class WHOOMan : public Thread {
 public:
	WHOOMan() : Thread() {}
	virtual void action();
};

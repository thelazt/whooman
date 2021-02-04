#pragma once
class Layout;

#include "def.h"
#include "playground.h"

class Layout {
 protected:
	void defaultLayout();
	void defaultPlayer(unsigned short players);

 public:
	virtual ~Layout() {}
	virtual void setup(unsigned short players) = 0;
};

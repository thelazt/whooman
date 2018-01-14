#ifndef _BOMB_LAYOUT_H
#define _BOMB_LAYOUT_H

class Layout;

#include "def.h"
#include "playground.h"

class Layout {

protected:
	void defaultLayout(Playground& ground);

	void defaultPlayer(Playground& ground, unsigned short players);

public: 
	virtual void setup(Playground& ground, unsigned short players);
};

#endif

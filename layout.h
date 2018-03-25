#ifndef _BOMB_LAYOUT_H
#define _BOMB_LAYOUT_H

class Layout;

#include "def.h"
#include "playground.h"

class Layout {

protected:
	void defaultLayout(Playground& ground);
	void defaultPlayer(Playground& ground, unsigned short players);
	void defaultItems(Playground& ground);

public:
	virtual ~Layout(){};
	virtual void setup(Playground& ground, unsigned short players);
};

#endif

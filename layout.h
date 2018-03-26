#ifndef _BOMB_LAYOUT_H
#define _BOMB_LAYOUT_H

class Layout;

#include "def.h"
#include "playground.h"

class Layout {

protected:
	void defaultLayout();
	void defaultPlayer(unsigned short players);

public:
	virtual ~Layout(){};
	virtual void setup(unsigned short players);
};

#endif

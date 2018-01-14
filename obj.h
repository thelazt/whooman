#ifndef BOMB_OBJ_H
#define BOMB_OBJ_H

#define INVALID 0xffff
const unsigned fieldPixel = 16;
const unsigned maxBombs = 16;


class Field {
public:
	static unsigned w, h;
	unsigned x, y;
	Field(unsigned _x, unsigned _y) : x(_x), y(_y) {
		if (x >= w || y >= h){
			x = INVALID;
			y = INVALID;
		}
	}
	Field() : x(INVALID), y(INVALID) {}
	bool valid(){
		return x < w && y < h;
	}
};

class Pos {
public:
	unsigned x, y;
	Pos(unsigned _x, unsigned _y) : x(_x), y(_y) {
		if (x >= Field::w * fieldPixel || y >= Field::h * fieldPixel){
			x = INVALID;
			y = INVALID;
		}
	}
	Pos() : x(INVALID), y(INVALID) {}
	Field getField(){
		return Field((x + (fieldPixel/2))/fieldPixel,(y + (fieldPixel/2))/fieldPixel); }
	bool valid(){
		return x < Field::w*fieldPixel && y < Field::h*fieldPixel;
	}
};


class Obj {
protected:
	Field field;
public:
	bool explodable, massive;
	Obj(Field _field, bool _explodable, bool _massive) : field(_field), explodable(_explodable), massive(_massive) {}
	Obj( bool _explodable, bool _massive) : field(), explodable(_explodable), massive(_massive) {}
	virtual Field getField() { return field; };
	virtual void draw() = 0;
};


class Block : public Obj {
public:
	Block(Field field) : Obj(field, true, true) {}
};

class Stone : public Obj {
public:
	Block(Field field) : Obj(field, false, true) {}
};


class Bomb;

class Player: public Obj {
	Pos pos;
	bool kick, box;

public:
	unsigned power;
	Bomb bombs[maxBombs];

	Player() : power(2), kick(false), box(false) {
		for (unsigned i = 0; i < maxBombs; i++){
			bombs[i].owner = this;
			bombs.enabled = i==0;
		}
	}

	getField() { 
		return pos.getField();
	}


};

class Item : public Obj {
	enum ItemType { POWER, BOMBS, KICK, BOX, SICKNESS };
	enum ItemType type;
public:
	Item(enum ItemType _type) : Obj(true, false), type(_type) {}
};

class Bomb: public Obj {
public:
	Player *owner;
	unsigned power;
	unsigned timer;
	bool enabled;
	Bomb() : Obj(true, true) {}
	bool isActive(){
		return power == 0;
	}
};

#endif

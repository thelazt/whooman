#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <climits>
#include <math.h>
#include <unistd.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


using namespace std;
const unsigned fieldPixel = 16;

const unsigned int screenWidth = 400;
const unsigned int screenHeight = 400;

const unsigned int fieldWidth = 15;
const unsigned int fieldHeight = 13;

#define CELL_ACCESSABLE 0xf0
enum CellType {
	CELL_WALL = 0,
	CELL_BLOCK = 1,
	CELL_BOMB = 2,
	CELL_FIRE = CELL_ACCESSABLE | 0,
	CELL_ITEM = CELL_ACCESSABLE | 1,
	CELL_GRASS = CELL_ACCESSABLE | 2
};
enum ItemType { ITEM_BOMB, ITEM_SPEED, ITEM_POWER, ITEM_SICK, ITEM_ULTRA };

enum PlayerDir { MOVE_UP = 3, MOVE_DOWN = 0, MOVE_LEFT = 1, MOVE_RIGHT = 2, MOVE_HEAVEN = 4};

typedef struct {
	unsigned short x;
	unsigned short y;
	unsigned short power;
	unsigned short bombs;
	unsigned short speed;
	unsigned short sickness: 2;
	enum PlayerDir dir : 3;
	unsigned short tick : 2;
} bomberman;

typedef union{
	struct {
		enum CellType type  : 8;
		unsigned int sprite : 8;
		unsigned int player : 2;
		unsigned int tick : 6;
		unsigned int extra : 8;
		/*
			Wall: Sprit Type
			Block: Item ?
			Bomb: Time:8 + Power:6
			Fire: Sprite Type
			Item: Type
		*/
	};
	unsigned int value;
} cell;

static bomberman player[4];
static cell field[fieldHeight][fieldWidth];
SDL_Surface *screen;
SDL_Event event;
SDL_Surface * img_bomb;
SDL_Surface * img_fire;
SDL_Surface * img_items;
SDL_Surface * img_player[4];
SDL_Surface * img_playground;

void drawSprite(SDL_Surface * img, unsigned int num, unsigned x, unsigned y){
	SDL_Rect rcDest = { x * fieldPixel, y * fieldPixel, fieldPixel, fieldPixel};
	SDL_Rect rcSrc = {  0,num * fieldPixel, fieldPixel, fieldPixel};
	int r = SDL_BlitSurface ( img, &rcSrc, screen, &rcDest );
	if (r != 0){
		cout << "SDL_BlitSurface Error: " <<IMG_GetError() <<endl;
		exit(1);
	}
}

void draw(){
	
	if (SDL_MUSTLOCK(screen)) {
		SDL_LockSurface(screen);
	}
	int inc = 1;

	for (int y = 0; y < fieldHeight; y++)
		for (int x = 0; x < fieldWidth; x++){
			cell &c = field[y][x];
			switch (c.type){
				case CELL_WALL:
					drawSprite(img_playground, 0, x, y);
					break;

				case CELL_BLOCK:
					drawSprite(img_playground, 1, x, y);
					break;

				case CELL_BOMB:
					drawSprite(img_playground, field[y-1][x].type <= CELL_BLOCK ? 2 : 3 , x, y);
					c.tick+= inc;
					drawSprite(img_bomb, c.tick % 4 == 3 ? 1 : c.tick % 4, x, y);
					break;

				case CELL_FIRE:
					drawSprite(img_playground, field[y-1][x].type <= CELL_BLOCK ? 2 : 3 , x, y);
					c.tick = (c.tick + inc) % 9;
					drawSprite(img_fire, c.tick > 4 ? (9 - c.tick) : c.tick, x, y);
					break;

				case CELL_ITEM:
					c.tick = (c.tick + inc) % 2;
					drawSprite(img_items, c.tick*9 + c.extra, x, y);
					break;
				case CELL_GRASS:
					drawSprite(img_playground, field[y-1][x].type <= CELL_BLOCK ? 2 : 3 , x, y);
					break;
			}
		}

	for (int p = 0; p < 4; p++){
		const int playerPixel = 24;
		bomberman &b = player[p];
		unsigned num;
		if (b.dir == MOVE_HEAVEN){
			num = b.tick %= 5;
			if (b.tick == 4)
				break;
		} else {
			b.tick %= 4;
			num = b.tick == 3 ? 2 : b.tick;
		}
		SDL_Rect rcDest = { b.x - fieldPixel/2 +1, b.y - fieldPixel -2, fieldPixel, playerPixel};
		SDL_Rect rcSrc = {  0, (b.dir * 3 + num) * playerPixel, fieldPixel, playerPixel};
		int r = SDL_BlitSurface(img_player[p], &rcSrc, screen, &rcDest );
		if (r != 0){
			cout << "SDL_BlitSurface Error: " <<IMG_GetError() <<endl;
			exit(1);
		}
	}

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}

	SDL_Flip(screen);
}

inline unsigned int min(unsigned int a, unsigned int b){
	return a < b ? a : b;
}

inline unsigned int max(unsigned int a, unsigned int b){
	return a > b ? a : b;
}


void move(int p, enum PlayerDir dir){
	const int figureSpace = 6;
	unsigned int x = 0,y = 0;
	switch(dir) {
		case MOVE_UP: y = -1; break;
		case MOVE_DOWN: y = 1; break;
		case MOVE_LEFT: x = -1; break;
		case MOVE_RIGHT: x = 1; break;
	}
	player[p].dir = dir;
	player[p].tick++;
	unsigned int fx = player[p].x/fieldPixel;
	unsigned int fy = player[p].y/fieldPixel;
	player[p].x = max(player[p].x + x, (field[fy][fx - 1].type & CELL_ACCESSABLE) ? 0 : fx * fieldPixel + figureSpace);
	player[p].x = min(player[p].x + x, (field[fy][fx + 1].type & CELL_ACCESSABLE) ? UINT_MAX : (fx+1) * fieldPixel - figureSpace -1);
	player[p].y = max(player[p].y + y, (field[fy - 1][fx].type & CELL_ACCESSABLE) ? 0 : fy * fieldPixel + figureSpace);
	player[p].y = min(player[p].y + y, (field[fy + 1][fx].type & CELL_ACCESSABLE) ? UINT_MAX : (fy+1) * fieldPixel - figureSpace -1);
}

void dropBomb(int p){
	unsigned int fx = player[p].x/fieldPixel;
	unsigned int fy = player[p].y/fieldPixel;
	field[fy][fx].type = CELL_BOMB;
}

SDL_Surface * loadImage(const char * path){
	SDL_Surface * target = IMG_Load (path );
	if (!target ){
		printf ( "IMG_Load: %s\n", IMG_GetError () );
		exit(1);
	}
	return target;
}

int main(int argc, char * argv[]){

	for (int p = 0; p < 4; p++){
		player[p].x = (p < 2 ? 1 : (fieldWidth - 2)) * fieldPixel + fieldPixel/2;
		player[p].y = (p % 2 ? 1 : (fieldHeight - 2)) * fieldPixel + fieldPixel/2;
		player[p].power = 1;
		player[p].bombs = 1;
		player[p].speed = 3;
		player[p].sickness = 0;
	}
	for (int y = 0; y < fieldHeight; y++)
		for (int x = 0; x < fieldWidth; x++){
			field[y][x].value = 0;
			// Outer Wals
			if (y == 0 || x == 0 || y == fieldHeight-1 || x == fieldWidth - 1){
				field[y][x].type = CELL_WALL;
				if (y == 0)
					field[y][x].sprite = x == 0 ? 0 : (x == fieldWidth - 1 ? 2 : 1);
				else if (y == fieldHeight-1)
					field[y][x].sprite = x == 0 ? 5 : (x == fieldWidth - 1 ? 7 : 6);
				else
					field[y][x].sprite = x == 0 ? 3 : 4;
			}
			// inner Walls
			else if (y % 2  == 0 && x % 2 == 0 ){
				field[y][x].type = CELL_WALL;
				field[y][x].sprite = 9;
			}
			// Blocks
			else if ((x > 2 && x < fieldWidth - 3) || (y > 2 && y < fieldHeight - 3)){
				field[y][x].type = CELL_BLOCK;
				// TODO: Item
			}
			// Grass
			else
				field[y][x].type = CELL_GRASS;
		}
	cout << sizeof(cell) << endl;
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(screenWidth, screenHeight, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("Bomberman", NULL);
	SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL);

	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ){
		cout << "SDL_image could not initialize! SDL_image Error: " <<IMG_GetError() <<endl;
		return 1;
	}

	img_bomb = loadImage("img_bomb.png");
	img_fire = loadImage("img_fire.png");
	img_items = loadImage("img_items.png");
	img_playground = loadImage("img_playground0.png");
	for (int p = 0; p < 4; p++)
		img_player[p] = loadImage("img_player1.png");

	int quit = 0;
	Uint32 blackColor = SDL_MapRGB(screen->format, 0, 0, 0);
	
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						move(0,MOVE_UP);
						break;
					case SDLK_DOWN:
						move(0,MOVE_DOWN);
						break;
					case SDLK_LEFT:
						move(0,MOVE_LEFT);
						break;
					case SDLK_RIGHT:
						move(0,MOVE_RIGHT);
						break;
					case SDLK_SPACE:
						dropBomb(0);
						break;
					case SDLK_ESCAPE:
						quit = 1;
						break;
					default:
						break;
				}
			}
			else if (event.type == SDL_QUIT) {
				quit = 1;
			}
		}
		SDL_FillRect(screen, NULL, blackColor);
		draw();
		usleep(100000);	
	}
	
	SDL_Quit();
    
	return 0;
}

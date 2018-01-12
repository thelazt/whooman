#include <iostream>
#include <stdint.h>
#include <climits>
#include <math.h>
#include <unistd.h>

#include "SDL/SDL.h"

using namespace std;
const unsigned fieldPixel = 16;

const unsigned int screenWidth = 400;
const unsigned int screenHeight = 400;

const unsigned int fieldWidth = 15;
const unsigned int fieldHeight = 13;

SDL_Surface *screen;
SDL_Event event;

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

typedef struct {
	unsigned short x;
	unsigned short y;
	unsigned short power;
	unsigned short bombs;
	unsigned short speed;
	unsigned short sickness;
} bomberman;

typedef union{
	struct {
		enum CellType type  : 8;
		unsigned int sprite : 8;
		unsigned int player : 2;
		unsigned int extra : 14;
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

void setField(int y, int x, Uint32 pixel) {
	Uint32 *pixels = (Uint32 *)screen->pixels;
	for (int ix = 0; ix < fieldPixel; ix++)
		for (int iy = 0; iy < fieldPixel; iy++)
			pixels[((y*fieldPixel + iy) * screen->w) + x*fieldPixel + ix] = pixel;
}

void setPlayer(int y, int x, Uint32 pixel) {
	Uint32 *pixels = (Uint32 *)screen->pixels;
	for (int ix = -5; ix <= 5; ix++)
		for (int iy = -5; iy <= 5; iy++)
			pixels[((y + iy) * screen->w) + x + ix] = pixel;
}

inline unsigned int min(unsigned int a, unsigned int b){
	return a < b ? a : b;
}

inline unsigned int max(unsigned int a, unsigned int b){
	return a > b ? a : b;
}


void move(int p, int y, int x){
	const int figureSpace = 5;
	unsigned int fx = player[p].x/fieldPixel;
	unsigned int fy = player[p].y/fieldPixel;
	if (x <= 0)
		player[p].x = max(player[p].x + x, (field[fy][fx - 1].type & CELL_ACCESSABLE) ? 0 : fx * fieldPixel + figureSpace);
	if (x >= 0)
		player[p].x = min(player[p].x + x, (field[fy][fx + 1].type & CELL_ACCESSABLE) ? UINT_MAX : (fx+1) * fieldPixel - figureSpace -1);
	if (y <= 0)
		player[p].y = max(player[p].y + y, (field[fy - 1][fx].type & CELL_ACCESSABLE) ? 0 : fy * fieldPixel + figureSpace);
	if (y >= 0)
		player[p].y = min(player[p].y + y, (field[fy + 1][fx].type & CELL_ACCESSABLE) ? UINT_MAX : (fy+1) * fieldPixel - figureSpace -1);
}

void dropBomb(int p){
	unsigned int fx = player[p].x/fieldPixel;
	unsigned int fy = player[p].y/fieldPixel;
	field[fy][fx].type = CELL_BOMB;
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
	
	int quit = 0;
	Uint32 blackColor = SDL_MapRGB(screen->format, 0, 0, 0);
	
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						move(0,-1,0);
						break;
					case SDLK_DOWN:
						move(0,1,0);
						break;
					case SDLK_LEFT:
						move(0,0,-1);
						break;
					case SDLK_RIGHT:
						move(0,0,1);
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
		
		if (SDL_MUSTLOCK(screen)) {
			SDL_LockSurface(screen);
		}
		for (int y = 0; y < fieldHeight; y++)
			for (int x = 0; x < fieldWidth; x++)
				setField(y, x, 5 + field[y][x].type * 50);
		for (int p = 0; p < 4; p++)
			setPlayer(player[p].y, player[p].x, p * 0x500000);
		if (SDL_MUSTLOCK(screen)) {
			SDL_UnlockSurface(screen);
		}
		
		SDL_Flip(screen);
		usleep(50);
	}
	
	SDL_Quit();
    
	return 0;
}

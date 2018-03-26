#ifndef _BOMB_GAME_H
#define _BOMB_GAME_H

#include "def.h"
#include "item.h"
#include "arena.h"
#include "layout.h"
#include "player.h"
#include "playground.h"

class Game {
public:
	enum ArenaName { ARENA_ATOMIC, ARENA_CASTLE, ARENA_CITY, ARENA_CLASSIC, ARENA_DIRT, ARENA_FACTORY, ARENA_HAL, ARENA_RETRO, ARENA_STATION, ARENA_WOOD, ARENA_RANDOM };
	enum LayoutName { LAYOUT_CLASSIC, LAYOUT_EMPTY, LAYOUT_RANDOM };

protected:
	enum Playground::GameState round(unsigned short _player, enum ArenaName arena, enum LayoutName layout, enum Item::ItemSet itemset, unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize);

	bool input(enum Player::PlayerDir (&move)[maxPlayer]);

	Arena * newArena(enum ArenaName _arena, unsigned short _offsetX = 0, unsigned short _offsetY = 0, unsigned short _tileSize = defaultTileSize) const;
	Layout * newLayout(enum LayoutName _layout) const;

public:
	bool play(unsigned short _player = maxPlayer, unsigned short rounds = 3, unsigned short width = 15, unsigned short height = 13, enum ArenaName _arena = ARENA_CLASSIC, enum LayoutName _layout = LAYOUT_CLASSIC, enum Item::ItemSet itemset = Item::SET_CLASSIC);
};

extern Game game;
#endif

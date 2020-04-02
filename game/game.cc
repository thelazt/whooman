#include "def.h"
#include "game.h"
#include "arenas.h"
#include "input.h"
#include "player.h"
#include "screen.h"
#include "layouts.h"
#include "utils.h"

bool Game::input(enum Player::PlayerDir (&move)[maxPlayer]) {
	if (Input::update()) {
		for (unsigned short p = 0; p < maxPlayer; p++) {
			if (player[p].input != Input::NONE) {
				move[p] = Player::MOVE_WAIT;
				if (Input::active(player[p].input, Input::PLACE_BOMB))
					player[p].bomb();
				for (int a = 0; a < Input::PLACE_BOMB; a++)
					if (Input::active(player[p].input, static_cast<Input::Action>(a)))
						move[p] = static_cast<Player::PlayerDir>(a);
			}
		}
		return true;
	}
	return false;
}

Arena * Game::newArena(enum ArenaName arena, unsigned short _offsetX, unsigned short _offsetY,
                       unsigned short _tileSize) const {
	switch(arena) {
		case ARENA_ATOMIC:
			return new ArenaAtomic(_offsetX, _offsetY, _tileSize);
		case ARENA_CASTLE:
			return new ArenaCastle(_offsetX, _offsetY, _tileSize);
		case ARENA_CITY:
			return new ArenaCity(_offsetX, _offsetY, _tileSize);
		case ARENA_DIRT:
			return new ArenaDirt(_offsetX, _offsetY, _tileSize);
		case ARENA_FACTORY:
			return new ArenaFactory(_offsetX, _offsetY, _tileSize);
		case ARENA_HAL:
			return new ArenaHAL(_offsetX, _offsetY, _tileSize);
		case ARENA_RETRO:
			return new ArenaRetro(_offsetX, _offsetY, _tileSize);
		case ARENA_STATION:
			return new ArenaStation(_offsetX, _offsetY, _tileSize);
		case ARENA_WOOD:
			return new ArenaWood(_offsetX, _offsetY, _tileSize);
		case ARENA_RANDOM:
			return newArena((enum ArenaName) (number() % ARENA_RANDOM), _offsetX, _offsetY, _tileSize);
		default:
			return new ArenaDefault(_offsetX, _offsetY, _tileSize);
	}
}

Layout * Game::newLayout(enum LayoutName layout) const {
	switch(layout) {
		case LAYOUT_RANDOM:
			return newLayout((enum LayoutName) (number() % LAYOUT_RANDOM));
		default:
			return new Layout();
	}
}

enum Playground::GameState Game::round(unsigned short _player, enum ArenaName _arena, enum LayoutName _layout,
                                       enum Item::ItemSet itemset, unsigned short _offsetX, unsigned short _offsetY,
                                       unsigned short _tileSize) {
	enum Playground::GameState state = Playground::GAME_ACTIVE;
	// Initialize Playground
	Arena * arena = newArena(_arena, _offsetX, _offsetY, _tileSize);
	Layout * layout = newLayout(_layout);
	if (!playground.create(arena, *layout, itemset, _player)) {
		state = Playground::GAME_ERROR;
	} else {
		// Initialize Player
		enum Player::PlayerDir move[maxPlayer];  // NOLINT
		for (unsigned short p = 0; p < _player; p++)
			move[p] = player[p].input == Input::NONE ? Player::MOVE_AUTO : Player::MOVE_WAIT;
		// Game loop
		unsigned short i = 0;
		while(state == Playground::GAME_ACTIVE) {
			// Get movements
			if (!input(move)) {
				state = Playground::GAME_ABORT;
			} else {
				GuardedBell bell;
				bell.set(static_cast<int>(SUBTICK_MS));
				// Move Player
				for (unsigned short p = 0; p < _player; p++)
					player[p].move(move[p]);
				// Tick Game
				if (i++ == SUBTICKS) {
					state = playground.tick();
					i = 0;
				}
				// Draw
				playground.draw();
				// Wait.
				bell.sleep();
			}
		}
		if (state == Playground::GAME_WON || state == Playground::GAME_DRAW) {
			for (unsigned short x = 0; x < 60; x++) {
				GuardedBell bell;
				bell.set(static_cast<int>(SUBTICK_MS) * static_cast<int>(SUBTICKS));
				playground.tick();
				// Draw
				playground.draw();
				// Wait.
				bell.sleep();
			}
		}
	}
	// clean up
	delete(arena);
	delete(layout);
	return state;
}

bool Game::play(unsigned short _player, unsigned short rounds, unsigned short width, unsigned short height,
                enum ArenaName arena, enum LayoutName layout, enum Item::ItemSet itemset) {
	// Adjust rounds
	if (rounds < 1)
		rounds = 1;
	else if (rounds > 9)
		rounds = 9;

	for (unsigned short p = 0; p < _player; p++)
		player[p].resetPoints();

	// Set playground
	playground.dim(width, height);
	// Calculate offsets
	const unsigned short offsetY = defaultStatsHeight;
	const unsigned short offsetX = (Screen::getWidth() - (width * defaultTileSize)) / 2;

	// Start rounds
	for (unsigned short r = 1; r <= rounds; r++)
		switch(round(_player, arena, layout, itemset, offsetX, offsetY, defaultTileSize)) {
			case Playground::GAME_DRAW:
				r--;
				break;
			case Playground::GAME_WON:
				break;
			default:
				return false;
		}

	// TODO: Show winner

	// return
	return true;
}

Game game;

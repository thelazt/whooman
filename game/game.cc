#include "def.h"
#include "game.h"
#include "arenas.h"
#include "input.h"
#include "player.h"
#include "screen.h"
#include "layouts.h"
#include "utils.h"

bool Game::input(enum Player::PlayerDir (&move)[maxPlayer]) {
	if (Input::update() != Input::CONTROL_EXIT) {
		for (unsigned short p = 0; p < maxPlayer; p++) {
			if (player[p].input != Input::NONE && player[p].input != Input::AI) {
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
			return newArena(static_cast<enum ArenaName>(number() % ARENA_RANDOM), _offsetX, _offsetY, _tileSize);
		default:
			return new ArenaDefault(_offsetX, _offsetY, _tileSize);
	}
}

Layout * Game::newLayout(enum LayoutName layout) const {
	switch(layout) {
		case LAYOUT_EMPTY:
			return new LayoutEmpty();
		case LAYOUT_DENSE:
			return new LayoutDense();
		case LAYOUT_SPARSE:
			return new LayoutSparse();
		case LAYOUT_PATCHY:
			return new LayoutPatchy();
		case LAYOUT_RSTART:
			return new LayoutRandomStart();
		case LAYOUT_RANDOM:
			return newLayout(static_cast<enum LayoutName>(number() % LAYOUT_RANDOM));
		default:
			return new LayoutClassic();
	}
}

void Game::draw(bool tick, bool msgDraw, unsigned short round, unsigned short rounds) {
	// Load Sprites
	static Sprite * roundTitle = nullptr;
	static Sprite * roundNumber = nullptr;
	static Sprite * drawGame = nullptr;

	if (roundTitle == nullptr) {
		roundTitle = new Sprite("img/game_round_title.png", 111, 13);
		assert(roundTitle != nullptr);
	}
	if (roundNumber == nullptr) {
		roundNumber = new Sprite("img/game_round_number.png", 12, 13);
		assert(roundNumber != nullptr);
	}
	assert(round <= 7 && rounds <= 7);
	if (drawGame == nullptr) {
		drawGame = new Sprite("img/game_draw.png", 300, 115);
		assert(drawGame != nullptr);
	}

	Screen::lock();

	playground.draw(tick);

	roundTitle->draw(0, 20, 735);
	roundNumber->draw(round - 1, 80, 735);
	roundNumber->draw(rounds - 1, 136, 735);

	if (msgDraw)
		drawGame->draw(0, 362, 300);

	Screen::unlock();
	Screen::flip();
}

enum Playground::GameState Game::round(unsigned short _player, unsigned short round, unsigned short rounds,
                                       enum ArenaName _arena, enum LayoutName _layout, enum Item::ItemSet itemset,
                                       unsigned short _offsetX, unsigned short _offsetY, unsigned short _tileSize) {
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
			move[p] = player[p].input == Input::AI ? Player::MOVE_AUTO : Player::MOVE_WAIT;

		// Game loop
		draw(false, false, round, rounds);
		GuardedBell::sleep(1000);
		Input::update();
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
				draw(i == 0, false, round, rounds);

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
				draw(true, state == Playground::GAME_DRAW, round, rounds);

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
		switch(round(_player, r, rounds, arena, layout, itemset, offsetX, offsetY, defaultTileSize)) {
			case Playground::GAME_DRAW:
				r--;
				break;
			case Playground::GAME_WON:
				break;
			default:
				return false;
		}

	// return
	return true;
}

Game game;

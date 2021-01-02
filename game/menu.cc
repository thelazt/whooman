#include "menu.h"

#include "player.h"
#include "utils.h"
#include "input.h"
#include "game.h"
#include "item.h"
#include "def.h"

Menu::Menu()
  : background("img/menu_background.png", 1024, 768),
    cursor("img/menu_cursor.png", 17, 24),
    podium("img/menu_podium.png", 192, 122),
    infoBox("img/menu_info.png", 517, 531),
    infoBoxAni("img/menu_infoani.png", 401, 405) {

	// Player setup
	for (unsigned short p = 0; p < maxPlayer; p++) {
		playerSkin[p] = new MenuItemPlayerSkin(p);
		playerInput[p] = new MenuItemPlayerInput(p);
		assert(playerSkin[p] != nullptr && playerInput != nullptr);
	}

	// Arena
	arenaSize = new MenuItemArenaSize();
	arenaName = new MenuItemArenaName(Game::ARENA_RANDOM);
	arenaLayout = new MenuItemArenaLayout();
	assert(arenaSize != nullptr && arenaName != nullptr && arenaLayout != nullptr);

	// Items
	itemSet = new MenuItemSet();
	assert(itemSet != nullptr);

	// Rounds
	round = new MenuItemRound();
	assert(round != nullptr);

	// Buttons
	buttonPlay = new MenuItemButton(MenuItemButton::BTN_PLAY);
	buttonInfo = new MenuItemButton(MenuItemButton::BTN_INFO);
	buttonQuit = new MenuItemButton(MenuItemButton::BTN_QUIT);
	assert(buttonPlay != nullptr && buttonInfo != nullptr && buttonQuit != nullptr);
}


void Menu::stats() {
	const unsigned short wleft = 416;
	bool ani = false;
	while (Input::update()) {
		ani = !ani;
		Screen::lock();
		background.draw(0, 0, 0);
		podium.draw(0, wleft, 450);
		player[0].skin->draw(1, wleft + 8, 440);
		player[2].skin->draw(12, wleft + 118, 450);
		podium.draw(1, wleft, 450);
		player[1].skin->draw(ani ? 15 : 16, wleft + 62, 410);
		Screen::unlock();
		Screen::flip();
		GuardedBell::sleep(300);
	}
}

void Menu::show() {
	unsigned tick = 0;
	unsigned fire = 0;
	int mouse_x, mouse_y;
	while (true) {
		// Show Menu
		do {
			enum Input::Control c = Input::update();

			// Keyboard
			MenuItem::navigate(Input::active(Input::KEYBOARD_ARROW, Input::MOVE_UP),
			                   Input::active(Input::KEYBOARD_ARROW, Input::MOVE_DOWN),
			                   Input::active(Input::KEYBOARD_ARROW, Input::MOVE_LEFT),
			                   Input::active(Input::KEYBOARD_ARROW, Input::MOVE_RIGHT),
			                   Input::active(Input::KEYBOARD_ARROW, Input::PLACE_BOMB) || c == Input::CONTROL_ENTER);

			// Mouse
			bool mouse_button = Input::mouse(mouse_x, mouse_y);
			bool mouse_hover = MenuItem::pointer(mouse_x, mouse_y, mouse_button);

			// Draw
			Screen::lock();

			background.draw(0, 0, 0);

			MenuItem::showAll();

			if (buttonInfo->pressed()) {
				unsigned x = 370 + number() % 5;
				unsigned y = 48 + number() % 5;
				if ((tick++ % 5) == 0)
					fire = number() % 3;
				infoBoxAni.draw(fire, x, y);
				infoBox.draw(0, 80, 220);
				infoBoxAni.draw(fire + 3, x, y);
			}

			cursor.draw((mouse_hover ? (mouse_button ? 1 : 0) : 2), mouse_x, mouse_y);

			Screen::unlock();
			Screen::flip();

			if (buttonQuit->pressed(true))
				return;

			GuardedBell::sleep(30);
		} while (!buttonPlay->pressed(true));

		// Reorder (and count) players
		int players = 0;
		for (unsigned p = 0; p < maxPlayer; p++) {
			// Swap non-active players to the back
			if (player[p].input == Input::NONE) {
				Sprite * skin = player[p].skin;
				for (unsigned q = p + 1; q < maxPlayer; q++) {
					if (player[q].input != Input::NONE) {
						player[p].input = player[q].input;
						player[q].input = Input::NONE;
						player[p].skin = player[q].skin;
						player[q].skin = skin;
						players++;
						break;
					}
				}
			} else {
				players++;
			}
		}
		assert(players >= 2);

		// Start new game
		if (game.play(players, round->get(), arenaSize->getWidth(), arenaSize->getHeight(),
		              arenaName->get(), arenaLayout->get(), itemSet->get())) {
			int res[3];
			for (int r = 0; r < 3; r++) {
				res[r] = -1;
				int max = -1;
				for (int p = 0; p < players; p++) {
					if ((r >= 1 && res[0] == p) || (r >= 2 && res[1] == p)) {
						continue;
					} else if (max < static_cast<int>(player[p].getPoints())) {
						res[r] = p;
						max = player[p].getPoints();
					}
				}
			}

			const unsigned short wleft = 416;
			bool ani = false;
			while (Input::update() == Input::CONTROL_NONE && !Input::mouse(mouse_x, mouse_y)) {
				ani = !ani;
				Screen::lock();
				background.draw(0, 0, 0);
				podium.draw(0, wleft, 450);
				assert(res[0] != -1);
				player[res[0]].skin->draw(1, wleft + 8, 440);
				if (players > 2) {
					assert(res[2] != -1);
					player[res[2]].skin->draw(12, wleft + 118, 450);
				}
				podium.draw(1, wleft, 450);
				assert(res[1] != -1);
				player[res[1]].skin->draw(ani ? 15 : 16, wleft + 62, 410);

				Screen::unlock();
				Screen::flip();
				GuardedBell::sleep(300);
			}
		}

	}

}

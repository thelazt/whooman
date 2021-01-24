#include "player.h"
#include "menu.h"
#include "game.h"
#include "whooman.h"
#include "machine/system.h"

void WHOOMan::action() {
	Menu menu;

	menu.show();

	System::reboot();
}

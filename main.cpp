#include <SDL3/SDL.h>
#include "Menu.h"

Menu* menu = nullptr;

int main()
{
	menu = new Menu;
	menu->runLoop();

	return 0;
}
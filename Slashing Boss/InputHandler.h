#pragma once

#include "hero.h"

class InputHandler
{
public:
	Hero* hero;
	SDL_Scancode UP, DOWN, LEFT, RIGHT;
	SDL_Scancode SLASH, DASH;

	InputHandler();
	void update(SDL_Event* e);
};
#include "InputHandler.h"

InputHandler::InputHandler()
{
	UP = SDL_SCANCODE_UP;
	DOWN = SDL_SCANCODE_DOWN;
	LEFT = SDL_SCANCODE_LEFT;
	RIGHT = SDL_SCANCODE_RIGHT;
	SLASH = SDL_SCANCODE_Z;
	DASH = SDL_SCANCODE_X;
}

void InputHandler::update(SDL_Event* e)
{
	if (e->type == SDL_KEYDOWN)
	{
		if (e->key.keysym.scancode == DASH) 
		{
			hero->dash();
		}
		if (e->key.keysym.scancode == SLASH)
		{
			hero->slash();
		}
	}

	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if ((hero->state != Hero::States::MOVE && hero->state != Hero::States::IDLE)
		|| (!keystates[UP] && !keystates[DOWN] && !keystates[LEFT] && !keystates[RIGHT]))
	{
		hero->moving = false;
	}
	else
	{
		if (keystates[UP])
		{
			if (keystates[RIGHT])
				hero->move(315);
			else if (keystates[LEFT])
				hero->move(225);
			else
				hero->move(270);
		}

		if (keystates[DOWN])
		{
			if (keystates[RIGHT])
				hero->move(45);
			else if (keystates[LEFT])
				hero->move(135);
			else
				hero->move(90);
		}

		if (!keystates[UP] && !keystates[DOWN] && !keystates[RIGHT] && keystates[LEFT])
			hero->move(180);
		if (!keystates[UP] && !keystates[DOWN] && keystates[RIGHT] && !keystates[LEFT])
			hero->move(0);

	}
}

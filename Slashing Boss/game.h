#pragma once

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "InputHandler.h"
#include "drawing_functions.h"

class Game 
{
public:
	AnimationSet* heroAnimSet;
	AnimationSet* wallAnimSet;

	SDL_Texture* bgImage;

	Hero* hero;
	InputHandler heroInput;

	list<Entity*> walls;

	Game();
	~Game();

	void update();
	void draw();
};
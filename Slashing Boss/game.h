#pragma once

#include "globals.h"
#include "hero.h"
#include "wall.h"
#include "glob.h"
#include "InputHandler.h"
#include "drawing_functions.h"
#include "soundManager.h"

class Game 
{
public:
	AnimationSet* heroAnimSet;
	AnimationSet* wallAnimSet;
	AnimationSet* globAnimSet;

	SDL_Texture* bgImage;
	SDL_Texture* splashScreen;
	SDL_Texture* overlayScreen;

	SDL_Texture* scoreTexture = NULL;

	bool splashShowing;
	float overlayTimer;

	Hero* hero;
	InputHandler heroInput;

	Mix_Music* backgroundSound;

	list<Entity*> walls;
	list<Entity*> globs;

	Game();
	~Game();

	void update();
	void draw();
};
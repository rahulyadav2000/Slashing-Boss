#pragma once

#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "drawing_functions.h"
#include "globals.h"
#include "groupBuilder.h"

using namespace std;

class Frame
{
public:
	int frameNumber;
	SDL_Rect clip;
	float duration;
	SDL_Point offset;

	list<Group*> frameData;

	void Draw(SDL_Texture* spriteSheet, float x, float y);

	void loadFrame(ifstream& file, list<DataGroupType>& groupTypes);
};
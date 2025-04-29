#pragma once

#include <iostream>
#include <string>
#include "SDL.h"
#include "randomNumber.h"

using namespace std;

class Globals {
public:
	static const float PI;

	static bool debgguing;

	static int ScreenWidth, ScreenHeight, ScreenScale;
	static SDL_Renderer* renderer;

	static string clipOffDataHeader(string line);
};
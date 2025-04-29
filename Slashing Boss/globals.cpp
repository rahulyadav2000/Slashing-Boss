#include "globals.h"

const float Globals::PI = 3.14159;

bool Globals::debgguing = true;

int Globals::ScreenWidth = 640, Globals::ScreenHeight = 352, Globals::ScreenScale = 1;
SDL_Renderer* Globals::renderer = NULL;

string Globals::clipOffDataHeader(string line)
{
	int pos = line.find(":", 0);
	if (pos != -1)
	{
		line = line.substr(pos + 1, line.length() - pos + 2);
	}
	return line;
}

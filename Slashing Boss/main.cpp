#include<iostream>
#include "cleanup.h"
#include "res_path.h"
#include "drawing_functions.h"
#include "SDL_mixer.h"
#include "globals.h"
#include "game.h"

using namespace std;

int main(int agrc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "Error initialising SDL" << endl;
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Slashing Boss", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		Globals::ScreenWidth , Globals::ScreenHeight, 
		SDL_WINDOW_SHOWN);

	if (window == nullptr)
	{
		SDL_Quit();
		cout << "Window Error" << endl;
		return 1;
	}

	Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Globals::renderer == nullptr)
	{
		cleanup(window);
		SDL_Quit();
		cout << "Renderer Error" << endl;
		return 1;
	}

	SDL_RenderSetLogicalSize(Globals::renderer, Globals::ScreenWidth, Globals::ScreenHeight);

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		SDL_Quit();
		cout << "SDL Image did not initialised" << endl;
		return 1;
	}

	if (TTF_Init() != 0)
	{
		SDL_Quit();
		cout << "SDL TTF did not initialised" << endl;
		return 1;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_Quit();
		cout << "Mixer did not initialised" << endl;
		return 1;
	}
	Game game;
	game.update();


	cleanup(Globals::renderer);
	cleanup(window);


	SDL_Quit();
	return 0;
}
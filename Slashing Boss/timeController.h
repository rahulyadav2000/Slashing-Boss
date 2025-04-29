#pragma once
#include <iostream>
#include "SDL.h"

enum GameStates : int
{
	PLAY_STATE = 0,
	PAUSE_STATE = 1
};

class TimeController
{
public:
	GameStates states;

	Uint32 lastUpdate;
	float deltaTime;

	int timeState;

	TimeController();
	void updateTime();
	void pause();
	void resume();
	void reset();

	static TimeController timeController;
};
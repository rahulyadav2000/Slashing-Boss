#include "timeController.h"

TimeController::TimeController()
{
	deltaTime = 0;
	lastUpdate = 0;
	timeState = GameStates::PLAY_STATE;
}

void TimeController::updateTime()
{
	if (timeState == GameStates::PAUSE_STATE)
	{
		deltaTime = 0;
	}
	else 
	{
		Uint32 timeDifference = SDL_GetTicks() - lastUpdate;
		deltaTime = timeDifference / 1000.00;
	}
	lastUpdate = SDL_GetTicks();
}

void TimeController::pause()
{
	timeState = GameStates::PAUSE_STATE;
}

void TimeController::resume()
{
	timeState = GameStates::PLAY_STATE;
}

void TimeController::reset()
{
	deltaTime = 0;
	lastUpdate = SDL_GetTicks();
}

TimeController TimeController::timeController;
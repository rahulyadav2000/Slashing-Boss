#pragma once

#include "entity.h"

class Wall : public Entity
{
public:
	Wall(AnimationSet* animSet);
	void update();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateCollisions(){ }
};
#pragma once

#include "animation.h"

class AnimationSet
{
public:
	~AnimationSet();

	string imageName;
	SDL_Texture* spriteSheet;
	SDL_Texture* whiteSpriteSheet = NULL;
	list<Animation> animations;

	Animation* getAnimation(string name);
	void loadAnimationSet(string fileName, list<DataGroupType>& groupTypes,
		bool setColourKey = false, int transparentPixelIndex = 0, bool createWhiteTexture = false);
};
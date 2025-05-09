#pragma once

#include "entity.h"
#include "livingEntity.h"
#include "soundManager.h"


class Hero : public LivingEntity
{
public:
	enum States : int
	{
		IDLE = 0,
		MOVE = 1,
		SLASH = 2,
		DASH = 3,
		DEAD = 4
	};

	static const string HERO_ANIM_UP;
	static const string HERO_ANIM_DOWN;
	static const string HERO_ANIM_LEFT;
	static const string HERO_ANIM_RIGHT;

	static const string HERO_ANIM_IDLE_UP;
	static const string HERO_ANIM_IDLE_DOWN;
	static const string HERO_ANIM_IDLE_LEFT;
	static const string HERO_ANIM_IDLE_RIGHT;

	static const string HERO_SLASH_ANIM_UP;
	static const string HERO_SLASH_ANIM_DOWN;
	static const string HERO_SLASH_ANIM_LEFT;
	static const string HERO_SLASH_ANIM_RIGHT;

	static const string HERO_DASH_ANIM_UP;
	static const string HERO_DASH_ANIM_DOWN;
	static const string HERO_DASH_ANIM_LEFT;
	static const string HERO_DASH_ANIM_RIGHT;
	static const string HERO_ANIM_DIE;

	Hero(AnimationSet* animSet);
	States states;
	void update();
	void slash();
	void dash();
	void die();
	void revive();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateDamages();
};
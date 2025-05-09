#pragma once

#include <cstdlib>
#include "livingEntity.h"
#include "soundManager.h"

class Glob : public LivingEntity
{
public:
	enum States : int
	{
		IDLE = 0,
		MOVE = 1,
		ATTACK = 2,
		TELEGRAPH = 3,
		DEAD = 4
	};
	
	enum AIStates : int
	{
		NORMAL = 0,
		CHASE = 1
	};

	static const string GLOB_ANIM_UP;
	static const string GLOB_ANIM_DOWN;
	static const string GLOB_ANIM_LEFT;
	static const string GLOB_ANIM_RIGHT;

	static const string GLOB_ANIM_IDLE_UP;
	static const string GLOB_ANIM_IDLE_DOWN;
	static const string GLOB_ANIM_IDLE_LEFT;
	static const string GLOB_ANIM_IDLE_RIGHT;

	static const string GLOB_ANIM_ATTACK_UP;
	static const string GLOB_ANIM_ATTACK_DOWN;
	static const string GLOB_ANIM_ATTACK_LEFT;
	static const string GLOB_ANIM_ATTACK_RIGHT;

	static const string GLOB_ANIM_TELEGRAPH_UP;
	static const string GLOB_ANIM_TELEGRAPH_DOWN;
	static const string GLOB_ANIM_TELEGRAPH_LEFT;
	static const string GLOB_ANIM_TELEGRAPH_RIGHT;
	static const string GLOB_ANIM_DIE;


	static int globsKilled;

	float thinkTimer = 0;
	LivingEntity* target = NULL;

	int aiState = AIStates::NORMAL;

	Glob(AnimationSet* animSet);
	void update();
	void think();
	void telegraph();
	void attack();
	void die();
	void findNearestTarget();
	void changeAnimation(int newState, bool resetFrameToBeginning);
	void updateAnimation();
	void updateDamages();
};
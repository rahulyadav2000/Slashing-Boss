#include "glob.h"

const string Glob::GLOB_ANIM_UP = "up";
const string Glob::GLOB_ANIM_DOWN = "down";
const string Glob::GLOB_ANIM_LEFT = "left";
const string Glob::GLOB_ANIM_RIGHT = "right";

const string Glob::GLOB_ANIM_IDLE_UP = "idleUp";
const string Glob::GLOB_ANIM_IDLE_DOWN = "idleDown";
const string Glob::GLOB_ANIM_IDLE_LEFT = "idleLeft";
const string Glob::GLOB_ANIM_IDLE_RIGHT = "idleRight";

const string Glob::GLOB_ANIM_ATTACK_UP = "attackUp";
const string Glob::GLOB_ANIM_ATTACK_DOWN = "attackDown";
const string Glob::GLOB_ANIM_ATTACK_LEFT = "attackLeft";
const string Glob::GLOB_ANIM_ATTACK_RIGHT = "attackRight";

const string Glob::GLOB_ANIM_TELEGRAPH_UP = "telegraphUp";
const string Glob::GLOB_ANIM_TELEGRAPH_DOWN = "telegraphDown";
const string Glob::GLOB_ANIM_TELEGRAPH_LEFT = "telegraphLeft";
const string Glob::GLOB_ANIM_TELEGRAPH_RIGHT = "telegraphRight";
const string Glob::GLOB_ANIM_DIE = "die";

int Glob::globsKilled = 0;

Glob::Glob(AnimationSet* animSet)
{
	this->animSet = animSet;

	type = "enemy";

	x = Globals::ScreenWidth / 2 + 40;
	y = Globals::ScreenHeight / 2 + 40;

	moveSpeed = 0;
	moveSpeedMax = 20;
	health = healthMax = 10 + (rand() % 20);
	damage = 0;

	collisionBoxW = 18;
	collisionBoxH = 20;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;

	collisionBoxYOffset = -18;

	direction = Direction::DOWN;
	changeAnimation(States::IDLE, true);

	updateCollisionBox();
}

void Glob::update()
{
	if (health < 1 && state != States::DEAD)
	{
		changeAnimation(States::DEAD, true);
		moving = false;
		die();
	}

	think();

	updateCollisionBox();
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}

void Glob::think()
{
	if (state == States::IDLE || state == States::MOVE)
	{
		thinkTimer -= TimeController::timeController.deltaTime;

		if (thinkTimer <= 0)
		{
			thinkTimer = rand() % 5;
			int action = rand() % 10;

			if (action < 3)
			{
				moving = false;
				aiState = AIStates::NORMAL;
				changeAnimation(States::IDLE, true);
			}
			else
			{
				findNearestTarget();
				if (target != NULL && target->health > 0)
				{
					float dist = Entity::distBwtTwoEntities(this, target);
					if (dist < 100)
					{
						telegraph();
						aiState = AIStates::NORMAL;
					}
					else
					{
						aiState = AIStates::CHASE;
						moving = true;
						changeAnimation(States::MOVE, state != States::MOVE);
					}
				}
				else
				{
					moving = false;
					aiState = AIStates::NORMAL;
					changeAnimation(States::IDLE, true);
				}
			}
		}
	}
	if (aiState == AIStates::CHASE && health > 0 && active)
	{
		angle = Entity::angleBwtTwoEntities(this, target);
		move(angle);
	}
}

void Glob::telegraph()
{
	moving = false;
	frameTimer = 0;
	changeAnimation(States::TELEGRAPH, true);
}

void Glob::attack()
{
	moving = false;
	frameTimer = 0;
	slideAmount = 100;
	slideAngle = angle;
	changeAnimation(States::ATTACK, true);
}

void Glob::die()
{
	moving = false;
	state = States::DEAD;
	changeAnimation(state, true);
	SoundManager::soundManager.playSound("enemyDie");

	Glob::globsKilled++;
}

void Glob::findNearestTarget()
{
	target = NULL;

	for (auto en = Entity::entities.begin(); en != Entity::entities.end(); en++)
	{
		if ((*en)->type == "hero" && (*en)->active)
		{
			if (target == NULL)
			{
				target = (LivingEntity*)(*en);
			}
			else
			{
				if (Entity::distBwtTwoEntities(this, (*en)) < Entity::distBwtTwoEntities(this, target))
				{
					target = (LivingEntity*)(*en);
				}
			}
		}
	}
}

void Glob::changeAnimation(int newState, bool resetFrameToBeginning)
{
	state = newState;

	if (state == States::IDLE)
	{
		if (direction == Direction::DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_DOWN);
		else if (direction == Direction::UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_UP);
		else if (direction == Direction::LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_LEFT);
		else if (direction == Direction::RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_IDLE_RIGHT);
	}
	else if (state == States::MOVE)
	{
		if (direction == Direction::DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_DOWN);
		else if (direction == Direction::UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_UP);
		else if (direction == Direction::LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_LEFT);
		else if (direction == Direction::RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_RIGHT);
	}
	else if (state == States::ATTACK)
	{
		if (direction == Direction::DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_DOWN);
		else if (direction == Direction::UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_UP);
		else if (direction == Direction::LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_LEFT);
		else if (direction == Direction::RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_ATTACK_RIGHT);
	}
	else if (state == States::TELEGRAPH)
	{
		if (direction == Direction::DOWN)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_DOWN);
		else if (direction == Direction::UP)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_UP);
		else if (direction == Direction::LEFT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_LEFT);
		else if (direction == Direction::RIGHT)
			currentAnim = animSet->getAnimation(GLOB_ANIM_TELEGRAPH_RIGHT);
	}
	else if (state == States::DEAD)
		currentAnim = animSet->getAnimation(GLOB_ANIM_DIE);

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void Glob::updateAnimation()
{
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	if (state == States::MOVE && !moving)
	{
		changeAnimation(States::IDLE, true);
	}

	if (state != States::MOVE && moving)
	{
		changeAnimation(States::MOVE, true);
	}

	frameTimer += TimeController::timeController.deltaTime;
	if (frameTimer >= currentFrame->duration)
	{
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber())
		{
			if (state == States::TELEGRAPH)
			{
				attack();
			}
			else if (state == States::ATTACK)
			{
				changeAnimation(States::MOVE, true);
			}
			else if (state == States::DEAD)
			{
				frameTimer = 0;
				if (health > 0)
				{
					changeAnimation(States::MOVE, true);
				}
				else
					active = false; 
			}
			else
			{
				currentFrame = currentAnim->getFrame(0);
			}
		}
		else
		{
			currentFrame = currentAnim->getNextFrame(currentFrame);
		}
		frameTimer = 0;
	}
}

void Glob::updateDamages()
{
	if (active && health > 0 && invincibleTimer <= 0)
	{
		for (auto en = Entity::entities.begin(); en != Entity::entities.end(); en++)
		{
			if ((*en)->active && (*en)->type == "hero")
			{
				LivingEntity* enemy = (LivingEntity*)(*en);

				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
				{
					health -= enemy->damage;
					if (health > 0)
					{
						invincibleTimer = 0.1;
						SoundManager::soundManager.playSound("enemyHit");
					}

					slideAngle = Entity::angleBwtTwoEntities((*en), this);
					slideAmount = 300;
				}
			}
		}
	}
}

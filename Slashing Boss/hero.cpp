#include "hero.h"

const string Hero::HERO_ANIM_UP = "up";
const string Hero::HERO_ANIM_DOWN = "down";
const string Hero::HERO_ANIM_LEFT = "left";
const string Hero::HERO_ANIM_RIGHT = "right";

const string Hero::HERO_ANIM_IDLE_UP = "idleUp";
const string Hero::HERO_ANIM_IDLE_DOWN = "idleDown";
const string Hero::HERO_ANIM_IDLE_LEFT = "idleLeft";
const string Hero::HERO_ANIM_IDLE_RIGHT = "idleRight";

const string Hero::HERO_SLASH_ANIM_UP = "slashUp";
const string Hero::HERO_SLASH_ANIM_DOWN = "slashDown";
const string Hero::HERO_SLASH_ANIM_LEFT = "slashLeft";
const string Hero::HERO_SLASH_ANIM_RIGHT = "slashRight";

const string Hero::HERO_DASH_ANIM_UP = "dashUp";
const string Hero::HERO_DASH_ANIM_DOWN = "dashDown";
const string Hero::HERO_DASH_ANIM_LEFT = "dashLeft";
const string Hero::HERO_DASH_ANIM_RIGHT = "dashRight";

const string Hero::HERO_ANIM_DIE = "die";

Hero::Hero(AnimationSet* animSet)
{
	this->animSet = animSet;
	type = "hero";

	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;

	moveSpeed = 0;
	moveSpeedMax = 60;
	health = healthMax = 20;
	damage = 0;
	collisionBoxW = 20;
	collisionBoxH = 24;
	collisionBoxYOffset = -20;

	direction = Direction::DOWN;

	changeAnimation(States::IDLE, true);
	
	updateCollisionBox();
}

void Hero::update()
{
	if (health < 1 && state != States::DEAD)
	{
		changeAnimation(States::DEAD, true);
		moving = false;
		die();
	}

	updateCollisionBox();
	updateMovement();
	updateCollisions();

	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}

void Hero::slash()
{
	if (health > 0 && (state == States::MOVE || state == States::IDLE))
	{
		moving = false;
		frameTimer = 0;
		changeAnimation(States::SLASH, true);
	}
}

void Hero::dash()
{
	if (health > 0 && (state == States::MOVE || state == States::IDLE))
	{
		moving = false;
		frameTimer = 0;
		slideAngle = angle;
		slideAmount = 250;
		invincibleTimer = 0.1;

		changeAnimation(States::DASH, true);
	}
}

void Hero::die()
{
	moving = false;
	changeAnimation(States::DEAD, true);
}

void Hero::revive()
{
	health = healthMax;
	changeAnimation(States::IDLE, true);
	moving = false;
	x = Globals::ScreenWidth / 2;
	y = Globals::ScreenHeight / 2;
	slideAmount = 0;
}

void Hero::changeAnimation(int newState, bool resetFrameToBeginning)
{
	state = newState;

	if (state == States::IDLE)
	{
		if (direction == Direction::DOWN) 
		{
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_DOWN);
		}
		else if (direction == Direction::UP)
		{
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_UP); 
		}
		else if (direction == Direction::LEFT)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_LEFT);
		else if (direction == Direction::RIGHT)
			currentAnim = animSet->getAnimation(HERO_ANIM_IDLE_RIGHT);
	}
	else if (state == States::MOVE)
	{
		if (direction == Direction::DOWN)
			currentAnim = animSet->getAnimation(HERO_ANIM_DOWN);
		else if (direction == Direction::UP)
			currentAnim = animSet->getAnimation(HERO_ANIM_UP);
		else if (direction == Direction::LEFT)
			currentAnim = animSet->getAnimation(HERO_ANIM_LEFT);
		else if (direction == Direction::RIGHT)
			currentAnim = animSet->getAnimation(HERO_ANIM_RIGHT);
	}
	else if (state == States::SLASH)
	{
		if (direction == Direction::DOWN)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_DOWN);
		else if (direction == Direction::UP)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_UP);
		else if (direction == Direction::LEFT)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_LEFT);
		else if (direction == Direction::RIGHT)
			currentAnim = animSet->getAnimation(HERO_SLASH_ANIM_RIGHT);
	}
	else if (state == States::DASH)
	{
		if (direction == Direction::DOWN)
			currentAnim = animSet->getAnimation(HERO_DASH_ANIM_DOWN);
		else if (direction == Direction::UP)
			currentAnim = animSet->getAnimation(HERO_DASH_ANIM_UP);
		else if (direction == Direction::LEFT)
			currentAnim = animSet->getAnimation(HERO_DASH_ANIM_LEFT);
		else if (direction == Direction::RIGHT)
			currentAnim = animSet->getAnimation(HERO_DASH_ANIM_RIGHT);
	}
	else if (state == States::DEAD)
		currentAnim = animSet->getAnimation(HERO_ANIM_DIE);

	if (resetFrameToBeginning)
		currentFrame = currentAnim->getFrame(0);
	else
		currentFrame = currentAnim->getFrame(currentFrame->frameNumber);
}

void Hero::updateAnimation()
{
	if (currentFrame == NULL || currentAnim == NULL)
		return;

	if (state == States::MOVE && !moving)
		changeAnimation(States::IDLE, true);

	if (state != States::MOVE && moving)
		changeAnimation(States::MOVE, true);

	frameTimer += TimeController::timeController.deltaTime;
	if (frameTimer >= currentFrame->duration)
	{
		if (currentFrame->frameNumber == currentAnim->getEndFrameNumber())
		{
			if (state == States::SLASH || state == States::DASH)
				changeAnimation(States::MOVE, true);
			else if (state == States::DEAD && health > 0)
			{
				changeAnimation(States::MOVE, true);
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

void Hero::updateDamages()
{
	if (active && health > 0 && invincibleTimer <= 0)
	{
		for (auto e = Entity::entities.begin(); e != Entity::entities.end(); e++)
		{
			if ((*e)->active && (*e)->type == "enemy")
			{
				LivingEntity* enemy = (LivingEntity*)(*e);

				if (enemy->damage > 0 && Entity::checkCollision(collisionBox, enemy->hitBox))
				{
					health -= enemy->damage;
					if (health > 0)
						invincibleTimer = 0.3;

					slideAngle = Entity::angleBwtTwoEntities((*e), this);
					slideAmount = 200;
				}
			}
		}
	}
}

#include "entity.h"

void Entity::update()
{
	// override 
}

void Entity::draw()
{
	if (currentFrame != NULL && active)
	{
		currentFrame->Draw(animSet->spriteSheet, x, y);
	}
	
	if (solid && Globals::debgguing)
	{
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);
	}
}

void Entity::move(float angle)
{
	moving = true;
	moveSpeed = moveSpeedMax;
	this->angle = angle;

	int newDir = angleToDir(angle);

	if (direction != newDir)
	{
		direction = newDir;
		changeAnimation(state, false);
	}
}

void Entity::updateMovement()
{
	updateCollisionBox();

	lastCollisionBox = collisionBox;
	totalXMove = 0;
	totalYMove = 0;

	if (moveSpeed > 0)
	{
		float moveDist = moveSpeed * TimeController::timeController.deltaTime * moveLerp;
		if (moveDist > 0)
		{
			float xMove = moveDist * (cos(angle * Globals::PI / 180));
			float yMove = moveDist * (sin(angle * Globals::PI / 180));

			x += xMove;
			y += yMove;

			totalXMove = xMove;
			totalYMove = yMove;

			if (!moving)
				moveSpeed -= moveDist;
		}
	}

	if (slideAmount > 0)
	{
		float slideDis = slideAmount * TimeController::timeController.deltaTime * moveLerp;
		if (slideDis > 0)
		{
			float xSlide = slideDis * (cos(slideAngle * Globals::PI / 180));
			float ySlide = slideDis * (sin(slideAngle * Globals::PI / 180));

			x += xSlide;
			y += ySlide;

			totalXMove += xSlide;
			totalYMove += ySlide;
			slideAmount -= slideDis;
		}
		else
		{
			slideAmount = 0;
		}
	}

	updateCollisionBox();

	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void Entity::updateCollisionBox()
{
	collisionBox.x = x - collisionBox.w / 2;
	collisionBox.y = y + collisionBoxYOffset;
	collisionBox.w = collisionBoxW;
	collisionBox.h = collisionBoxH;
}

void Entity::updateCollisions()
{
	if (active && collideWithSolids && (moveSpeed > 0 || slideAmount > 0))
	{
		list<Entity*> collisions;

		for (auto e = Entity::entities.begin(); e != Entity::entities.end(); e++)
		{
			if ((*e)->active && (*e)->type != this->type && (*e)->solid && Entity::checkCollision(collisionBox, (*e)->collisionBox))
			{
				collisions.push_back(*e);
			}
		}

		if (collisions.size() > 0)
		{
			float boxTravelSize = 0;
			if (collisionBox.w > collisionBox.h)
			{
				boxTravelSize = collisionBox.w / 4;
			}
			else
				boxTravelSize = collisionBox.h / 4;

			SDL_Rect sampleBox = lastCollisionBox;
			float sampleCollisionX = sampleBox.x, sampleCollisionY = sampleBox.y;
			float movementAngle = Entity::angleBwtTwoRects(lastCollisionBox, collisionBox);

			bool foundCollision = false;
			while (!foundCollision)
			{
				SDL_Rect intersection;
				for (auto en = collisions.begin(); en != collisions.end(); en++)
				{
					if (SDL_IntersectRect(&sampleBox, &(*en)->collisionBox, &intersection))
					{
						foundCollision = true;
						moveSpeed = 0;
						moving = false;
						slideAngle = angleBwtTwoEntities((*en), this);

						if (intersection.w < intersection.h)
						{
							if (lastCollisionBox.x + lastCollisionBox.w / 2 < (*en)->collisionBox.x + (*en)->collisionBox.w / 2)
								sampleBox.x -= intersection.w; // left
							else
								sampleBox.x += intersection.w; // right
						}
						else
						{
							if (lastCollisionBox.x + lastCollisionBox.w / 2 < (*en)->collisionBox.x + (*en)->collisionBox.w / 2)
								sampleBox.y -= intersection.y; // up
							else
								sampleBox.y -= intersection.h; // down

						}
					}
				}
				if (foundCollision || (sampleBox.x == collisionBox.x && sampleBox.y || collisionBox.y))
					break;

				if (distBwtTwoRects(sampleBox, collisionBox) > boxTravelSize)
				{
					movementAngle = Entity::angleBwtTwoRects(sampleBox, collisionBox);
					float xMove = boxTravelSize * (cos(movementAngle * Globals::PI / 180));
					float yMove = boxTravelSize * (sin(movementAngle * Globals::PI / 180));

					sampleCollisionX += xMove;
					sampleCollisionY += yMove;
					sampleBox.x = sampleCollisionX;
					sampleBox.y = sampleCollisionY;
				}
				else
					sampleBox = collisionBox;
			}

			if (foundCollision)
			{
				slideAmount = slideAmount / 2;
				x = sampleBox.x + sampleBox.w / 2;
				y = sampleBox.y + sampleBox.h / 2;
			}

			updateCollisionBox();
		}
	}
}

float Entity::distBwtTwoRects(SDL_Rect& r1, SDL_Rect& r2)
{
	SDL_Point e1, e2;
	e1.x = r1.x + r1.w / 2;
	e1.y = r1.y + r1.h / 2;

	e2.x = r2.x + r2.w / 2;
	e2.y = r2.y + r2.h / 2;

	float d = abs(sqrt(pow(e2.x - e1.x, 2) + pow(e2.y - e1.y, 2)));
	return d;
}

float Entity::distBwtTwoEntities(Entity* e1, Entity* e2)
{
	float d = abs(sqrt(pow(e2->x - e1->x, 2) + pow(e2->y - e1->y, 2)));
	return d;
}

float Entity::angleBwtTwoEntities(Entity* e1, Entity* e2)
{
	float dx, dy;
	float x1 = e1->x, y1 = e1->y, x2 = e2->x, y2 = e2->y;

	dx = x2 - x1;
	dy = y2 - y1;
	return atan2(dy, dx) * 180 / Globals::PI;
}

bool Entity::checkCollision(SDL_Rect cBox1, SDL_Rect cBox2)
{
	SDL_Rect intersection;
	if (SDL_IntersectRect(&cBox1, &cBox2, &intersection))
	{
		return true;
	}

	return false;
}

int Entity::angleToDir(float angle)
{
	if ((angle >= 0 && angle <= 45) || (angle >= 315 && angle <= 360))
		return Direction::RIGHT;
	else if (angle >= 45 && angle <= 135)
		return Direction::DOWN;
	else if (angle >= 135 && angle <= 225)
		return Direction::LEFT;
	else
		return Direction::UP;
}

float Entity::angleBwtTwoPoints(float cx1, float cy1, float cx2, float cy2)
{
	float dx = cx2 - cx1;
	float dy = cy2 - cy1;

	return atan2(dy, dx) * 180 / Globals::PI;
}

float Entity::angleBwtTwoRects(SDL_Rect& r1, SDL_Rect& r2)
{
	float cx1 = r1.x + (r1.w / 2);
	float cy1 = r1.y + (r1.h / 2);

	float cx2 = r2.x + (r2.w / 2);
	float cy2 = r2.y + (r2.h / 2);

	return angleBwtTwoPoints(cx1, cy1, cx2, cy2);
}

list<Entity*> Entity::entities;

bool Entity::EntityCompare(const Entity* const& a, const Entity* const& b)
{
	if (a != 0 && b != 0)
	{
		return (a->y < b->y);
	}
	else {
		return false;
	}
}

void Entity::removeInactiveEntitiesFromList(list<Entity*>* entityList, bool deleteEntities)
{
	for (auto e = entityList->begin(); e != entityList->end();)
	{
		if (!(*e)->active)
		{
			if (deleteEntities)
			{
				delete(*e);
				e = entityList->erase(e);
			}
		}
		else
		{
			e++;
		}
	}
}

void Entity::removeAllFromList(list<Entity*>* entityList, bool deleteEntities)
{
	for (auto e = entityList->begin(); e != entityList->end();)
	{
		if (deleteEntities)
		{
			delete(*e);
			e = entityList->erase(e);
		}
	}
}

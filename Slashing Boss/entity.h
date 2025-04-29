#pragma once

#include "globals.h"
#include "timeController.h"
#include "animationSet.h"



class Entity
{
public:

	enum Direction : int
	{
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
		NONE = -1
	};

	Direction dir;

	int state;

	float x, y;
	int direction;
	bool solid = true; 
	bool collideWithSolids = true;
	bool active = true;
	string type = "entity";
	bool moving;
	float angle;
	float moveSpeed;
	float moveSpeedMax;
	float slideAngle;
	float slideAmount;
	float moveLerp = 4;
	float totalXMove, totalYMove;

	SDL_Rect collisionBox;
	SDL_Rect lastCollisionBox;
	int collisionBoxW, collisionBoxH;
	float collisionBoxYOffset;

	AnimationSet* animSet;
	Animation* currentAnim;
	Frame* currentFrame;
	float frameTimer;

	virtual void  update();
	virtual void draw();

	virtual void move(float angle);
	virtual void updateMovement();
	virtual void updateCollisionBox();
	virtual void changeAnimation(int newState, bool resetFrame) = 0;
	virtual void updateCollisions();

	static float distBwtTwoRects(SDL_Rect& r1, SDL_Rect& r2);
	static float distBwtTwoEntities(Entity* e1, Entity* e2);
	static float angleBwtTwoEntities(Entity* e1, Entity* e2);
	static bool checkCollision(SDL_Rect cBox1, SDL_Rect cBox2);
	static int angleToDir(float angle);
	static float angleBwtTwoPoints(float cx1, float cy1, float cx2, float cy2);
	static float angleBwtTwoRects(SDL_Rect& r1, SDL_Rect& r2);

	static list<Entity*> entities;
	static bool EntityCompare(const Entity* const& a, const Entity* const& b);
	static void removeInactiveEntitiesFromList(list<Entity*>* entityList, bool deleteEntities);
	static void removeAllFromList(list<Entity*> *entityList, bool deleteEntities);
};
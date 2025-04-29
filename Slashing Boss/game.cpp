#include "game.h"

Game::Game()
{
	string resPath = getResourcePath();
	bgImage = loadTexture(resPath + "map.png", Globals::renderer);

	list<DataGroupType> dateGroupTypes;

	DataGroupType colBoxType;
	colBoxType.groupName = "collisionBox";
	colBoxType.dataType = DataGroupType::DATATYPE_BOX;

	DataGroupType hitBoxType;
	hitBoxType.groupName = "hitBox";
	hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

	DataGroupType dmgType;
	dmgType.groupName = "damage";
	dmgType.dataType = DataGroupType::DATATYPE_NUMBER;

	dateGroupTypes.push_back(colBoxType);
	dateGroupTypes.push_back(hitBoxType);
	dateGroupTypes.push_back(dmgType);

	heroAnimSet = new AnimationSet();
	heroAnimSet->loadAnimationSet("udemyCyborg.fdset", dateGroupTypes, true, 0, true);

	wallAnimSet = new AnimationSet();
	wallAnimSet->loadAnimationSet("wall.fdset", dateGroupTypes);

	hero = new Hero(heroAnimSet);
	hero->invincibleTimer = 0;
	hero->x = Globals::ScreenWidth / 2;
	hero->y = Globals::ScreenHeight / 2;

	heroInput.hero = hero;

	Entity::entities.push_back(hero);

	int tileSize = 32;

	// top walls
	for (int i = 0; i < Globals::ScreenWidth / tileSize; i++)
	{
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall-> y = tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		newWall = new Wall(wallAnimSet);
		newWall->x = i * tileSize + tileSize / 2;
		newWall->y = Globals::ScreenHeight - tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}

	for (int i = 1; i < Globals::ScreenHeight / tileSize - 1; i++)
	{
		Wall* newWall = new Wall(wallAnimSet);
		newWall->x = tileSize / 2;
		newWall->y = i * tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);

		newWall = new Wall(wallAnimSet);
		newWall->x = Globals::ScreenWidth - tileSize / 2;
		newWall->y = i * tileSize + tileSize / 2;
		walls.push_back(newWall);
		Entity::entities.push_back(newWall);
	}
}

Game::~Game()
{
	cleanup(bgImage);
	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimSet;
	delete wallAnimSet;

	delete hero;

	Entity::removeAllFromList(&walls, true);
}

void Game::update()
{
	bool quit = false;

	SDL_Event e;

	TimeController::timeController.reset();

	while (!quit)
	{
		TimeController::timeController.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				quit = true;

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					quit = true;
					break;
				case SDL_SCANCODE_LSHIFT:
					hero->revive();
					break;
				}
			}
			heroInput.update(&e);
		}
		for (list<Entity*>::iterator en = Entity::entities.begin(); en != Entity::entities.end(); en++)
		{
			(*en)->update();
		}

		draw();
	}
}

void Game::draw()
{
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, 255);
	SDL_RenderClear(Globals::renderer);

	renderTexture(bgImage, Globals::renderer, 0, 0);

	Entity::entities.sort(Entity::EntityCompare);

	for (list<Entity*>::iterator en = Entity::entities.begin(); en != Entity::entities.end(); en++)
	{
		(*en)->draw();
	}

	SDL_RenderPresent(Globals::renderer);
}

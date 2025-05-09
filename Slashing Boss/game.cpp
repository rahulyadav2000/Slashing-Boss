#include "game.h"

Game::Game()
{
	string resPath = getResourcePath();
	bgImage = loadTexture(resPath + "map.png", Globals::renderer);
	splashScreen = loadTexture(resPath + "title.jpg", Globals::renderer);
	overlayScreen = loadTexture(resPath + "overlay.png", Globals::renderer);

	SoundManager::soundManager.loadSound("hit", resPath + "Randomize2.wav");
	SoundManager::soundManager.loadSound("enemyHit", resPath + "Hit_Hurt9.wav");
	SoundManager::soundManager.loadSound("swing", resPath + "Randomize21.wav");
	SoundManager::soundManager.loadSound("dash", resPath + "dash.wav");
	SoundManager::soundManager.loadSound("growl", resPath + "Randomize34.wav");
	SoundManager::soundManager.loadSound("enemyDie", resPath + "Randomize41.wav");

	backgroundSound = Mix_LoadMUS(string(resPath + "Fatal Theory.wav").c_str());
	if (backgroundSound != NULL)
	{
		Mix_PlayMusic(backgroundSound, -1);
	}

	splashShowing = true;
	overlayTimer = 2.f;

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
	heroAnimSet->loadAnimationSet("ruleSets.fdset", dateGroupTypes, true, 0, true);

	globAnimSet = new AnimationSet();
	globAnimSet->loadAnimationSet("glob.fdset", dateGroupTypes, true, 0, true);

	wallAnimSet = new AnimationSet();
	wallAnimSet->loadAnimationSet("wall.fdset", dateGroupTypes);

	hero = new Hero(heroAnimSet);
	hero->invincibleTimer = 0;
	hero->x = Globals::ScreenWidth / 2;
	hero->y = Globals::ScreenHeight / 2;

	heroInput.hero = hero;

	Entity::entities.push_back(hero);

	int tileSize = 32;


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
	cleanup(splashScreen);
	cleanup(overlayScreen);

	if (scoreTexture != NULL)
		cleanup(scoreTexture); 

	Mix_PauseMusic();
	Mix_FreeMusic(backgroundSound);

	Entity::removeAllFromList(&Entity::entities, false);

	delete heroAnimSet;
	delete wallAnimSet;
	delete globAnimSet;

	delete hero;

	Entity::removeAllFromList(&globs, true);
	Entity::removeAllFromList(&walls, true);
}

void Game::update()
{
	// enemies
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemiesBuildTimer = 1;

	bool quit = false;

	SDL_Event e;

	TimeController::timeController.reset();

	while (!quit)
	{
		TimeController::timeController.updateTime();

		Entity::removeInactiveEntitiesFromList(&Entity::entities, false);
		Entity::removeInactiveEntitiesFromList(&globs, true);

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
					if (splashScreen)
						splashShowing = false;  
					if (overlayTimer <= 0 && hero->health <= 1) {
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemiesBuildTimer =43;
						overlayTimer = 2;

						Glob::globsKilled = 0;
						if (scoreTexture != NULL)
						{
							cleanup(scoreTexture);
							scoreTexture = NULL;
						}

						for (list<Entity*>::iterator enemy = globs.begin(); enemy != globs.end(); enemy++)
						{
							(*enemy)->active = false;
						}
						hero->revive();
					}
					break;
				}
			}
			heroInput.update(&e);
		}

		if (hero->health <= 0 && overlayTimer >0)
		{
			overlayTimer -= TimeController::timeController.deltaTime;
		}

		for (list<Entity*>::iterator en = Entity::entities.begin(); en != Entity::entities.end(); en++)
		{
			(*en)->update();
		}

		if (hero->health > 0)
		{
			if (enemiesToBuild == enemiesBuilt)
			{
				enemiesToBuild = enemiesToBuild * 2;
				enemiesBuilt = 0;
				enemiesBuildTimer = 4;
			}

			enemiesBuildTimer -= TimeController::timeController.deltaTime;
			if (enemiesBuildTimer <= 0 && enemiesBuilt < enemiesToBuild && globs.size() < 10)
			{
				Glob* globEnemy = new Glob(globAnimSet);

				globEnemy->x = getRandomNumber(Globals::ScreenWidth - 32) + 48;
				globEnemy->y = getRandomNumber(Globals::ScreenHeight - 32) + 48;
				globEnemy->invincibleTimer = 0.1;

				globs.push_back(globEnemy);
				Entity::entities.push_back(globEnemy);
			}
		}

		draw();
	}
}

void Game::draw()
{
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, 255);
	SDL_RenderClear(Globals::renderer);

	if (splashShowing) {
		renderTexture(splashScreen, Globals::renderer, 0, 0);
	}
	else {

		renderTexture(bgImage, Globals::renderer, 0, 0);

		Entity::entities.sort(Entity::EntityCompare);

		for (list<Entity*>::iterator en = Entity::entities.begin(); en != Entity::entities.end(); en++)
		{
			(*en)->draw();
		}

		if (overlayTimer <= 0 && hero->health <= 0)
		{
			renderTexture(overlayScreen, Globals::renderer, 0, 0);
			if (scoreTexture == NULL)
			{
				SDL_Color color = { 255,255,255,255 };

				stringstream ss;
				ss << "Enemies Killed: " << Glob::globsKilled;

				string resPath = getResourcePath();
				scoreTexture = renderText(ss.str(), resPath + "Amatic-Bold.ttf", color, 30, Globals::renderer);
			}
			renderTexture(scoreTexture, Globals::renderer, 30, 50);
		}
	}

	SDL_RenderPresent(Globals::renderer);
}

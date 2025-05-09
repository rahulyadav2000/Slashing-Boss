#include "soundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	for (list<soundListing>::iterator s = sounds.begin(); s != sounds.end(); s++)
	{
		Mix_FreeChunk(s->sound);
	}
}

void SoundManager::loadSound(string name, string file)
{
	soundListing listing;
	listing.name = name;
	listing.sound = Mix_LoadWAV(file.c_str());

	sounds.push_back(listing);
}

void SoundManager::playSound(string name)
{
	for (list<soundListing>::iterator s = sounds.begin(); s != sounds.end(); s++)
	{
		if (s->name == name)
		{
			Mix_PlayChannel(-1, s->sound, 0);
			break;
		}
	}
}

SoundManager SoundManager::soundManager;

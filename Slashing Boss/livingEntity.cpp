#include "livingEntity.h"

void LivingEntity::updateHitBox()
{
	damage = 0;

	GroupBox* hitboxes = (GroupBox*)GroupBuilder::findGroupByName("hitBox", currentFrame->frameData);
	if (hitboxes != NULL && hitboxes->numberOfDataInGroup() > 0)
	{
		SDL_Rect hb = hitboxes->data.front();
		hitBox.x = x - currentFrame->offset.x + hb.x;
		hitBox.y = y - currentFrame->offset.y + hb.y;
		hitBox.w = hb.w;
		hitBox.h = hb.h;

		GroupNumber* damages = (GroupNumber*)GroupBuilder::findGroupByName("damage", currentFrame->frameData);
		if (damages != NULL && damages->numberOfDataInGroup() > 0)
		{
			damage = damages->data.front();
		}
	}
}

void LivingEntity::updateInvincibleTimer()
{
	if (invincibleTimer > 0)
	{
		invincibleTimer -= TimeController::timeController.deltaTime;
	}
}


void LivingEntity::draw()
{
	if (currentFrame != NULL && active)
	{
		if(invincibleTimer>0 && animSet->whiteSpriteSheet!=NULL)
			currentFrame->Draw(animSet->whiteSpriteSheet, x, y);
		else
			currentFrame->Draw(animSet->spriteSheet, x, y);
	}

	if (solid && Globals::debgguing)
	{
		SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, 255);
		SDL_RenderDrawRect(Globals::renderer, &lastCollisionBox);
		
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(Globals::renderer, &collisionBox);
		
		SDL_SetRenderDrawColor(Globals::renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(Globals::renderer, &hitBox);
	}
}

#include "PowerUp.h"

PowerUp::PowerUp(float xPos, float yPos)
{
	SetPosition(xPos, yPos - ITEM_BBOX_WIDTH/2);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_ITEM_POWERUP));
	typeItem = EntityType::TAG_ITEM_POWER_UP;
	
	displayTimer = new Timer(ITEM_POWER_UP_LIFETIME);
	displayTimer->Start();

}

PowerUp::~PowerUp()
{
}

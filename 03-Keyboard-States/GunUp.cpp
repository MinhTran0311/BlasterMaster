#include "GunUp.h"

GunUp::GunUp(float xPos, float yPos)
{
	SetPosition(xPos, yPos);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_ITEM_GUNUP));
	typeItem = EntityType::TAG_ITEM_GUN_UP;
	
	displayTimer = new Timer(ITEM_GUN_UP_LIFETIME);
	displayTimer->Start();
	
}

GunUp::~GunUp()
{
}

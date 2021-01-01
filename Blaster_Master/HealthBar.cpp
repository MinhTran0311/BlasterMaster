#include "HealthBar.h"

HealthBar::HealthBar(int initGunHealth, int isGun)
{
	maxLevel = 8;
	currentLevel = initGunHealth;
	this->isGun = isGun;
	ani_set = CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYERHP);
}

HealthBar::~HealthBar()
{}

void HealthBar::Update(int current, float xPos, float yPos)
{
	currentLevel = current;
	if (current <= 0) return;
	x = xPos + 1.5;
	if (isGun)
	{
		y = yPos;
	}
	else
	{
		y = yPos + float(HEALTH_BAR_HEIGHT + HEALTH_CHAR_HEIGHT + HEALTH_BLANK_SPACE * 2);
	}
}

void HealthBar::Render()
{
	if (currentLevel <= 0) return;
	for (int i = 0; i < currentLevel; i++)
	{
		ani_set->at(HEALTH_TYPE_UNIT)->Render(1, x, y + 29 - i * HEALTH_SPACE_UNIT);
	}
	
}

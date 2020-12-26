#include "HealthBar.h"

HealthBar::HealthBar(int maxHealth, int isGun)
{
	maxPlayerHealth = maxHealth;
	currentPlayerHealth = maxHealth;
	this->isGun = isGun;

	for (int i = 0; i < currentPlayerHealth; i++)
	{
		LPANIMATION_SET aniSet = CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYERHP);
		health.push_back(aniSet);
	}
}

HealthBar::~HealthBar()
{}

void HealthBar::Update(int currentHealth, float xPos, float yPos)
{
	currentPlayerHealth = currentHealth;
	if (currentHealth <= 0) return;
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
	if (currentPlayerHealth <= 0) return;
	else
	{
		for (int i = 0; i < currentPlayerHealth; i++)
		{
			if (isGun)
			{
				health[i]->at(HEALTH_TYPE_UNIT)->Render(1, x, y + 29 - i * HEALTH_SPACE_UNIT);
			}
			else
			{
				health[i]->at(HEALTH_TYPE_UNIT)->Render(1, x, y + 29 - i * HEALTH_SPACE_UNIT);
			}
		}
	}
}

#include "HUD.h"
HUD* HUD::instance = NULL;
HUD* HUD::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new HUD();
	}
	return instance;
}

HUD::~HUD()
{

}

void HUD::HUDInit(int initPlayerHealth, int initGunHealth)
{
	animationSet = CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYERHP);
	gunHB = new HealthBar(initGunHealth, true);
	playerHB = new HealthBar(initPlayerHealth, false);
}

void HUD::Update(float xPos, float yPos, int currentPlayerHealth, int currentGunHealth, EntityType playerType)
{
	this->x = xPos;
	this->y = yPos;

	playerHB->Update(currentPlayerHealth, x, y); 
	if (playerType==TAG_BIG_SOPHIA)
		gunHB->Update(currentGunHealth, x, y);
}

void HUD::Render(LPGAMEPLAYER playerInfo)
{
	//Thanh sung hoac chu HOV
	if (playerInfo->GetPlayerType() != TAG_BIG_SOPHIA)
	{
		animationSet->at(HEALTH_TYPE_CHAR_HOV)->Render(1, x, y + HEALTH_BAR_HEIGHT + HEALTH_BLANK_SPACE);
	}
	else
	{
		animationSet->at(HEALTH_TYPE_GUN_NULL)->Render(1, x, y);
		animationSet->at(HEALTH_TYPE_CHAR_HOV)->Render(1, x, y + HEALTH_BAR_HEIGHT + HEALTH_BLANK_SPACE);
		gunHB->Render();
	}
	//thanh Power
	animationSet->at(HEALTH_TYPE_PLAYER_NULL)->Render(1, x, y + HEALTH_BAR_HEIGHT + HEALTH_BLANK_SPACE * 2 + HEALTH_CHAR_HEIGHT);
	animationSet->at(HEALTH_TYPE_CHAR_POW)->Render(1, x, y + HEALTH_BAR_HEIGHT * 2 + HEALTH_BLANK_SPACE * 3 + HEALTH_CHAR_HEIGHT);
	playerHB->Render();
}

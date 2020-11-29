#include "HUD.h"

HUD::HUD(int initPlayerHealth, int initGunHealth)
{
	UIanimationSet = CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYERHP);
	gunHB = new HealthBar(initGunHealth, true);
	playerHB = new HealthBar(initPlayerHealth, false);
}

HUD::~HUD()
{
}

void HUD::Update(float x, float y, int currentPlayerHealth, int currentGunHealth)
{
	this->x = x;
	this->y = y;

	gunHB->Update(currentGunHealth, x + 2, y + 7.8); // Hard code -> Dieu chinh khoang cach cac unit mau cua Gun #LKP
	playerHB->Update(currentPlayerHealth, x + 3, y + 112.5); // Hard code -> Dieu chinh khoang cach cac unit mau cua Player #LKP
}

void HUD::Render(LPGAMEPLAYER playerInfo)
{
	UIanimationSet->at(HEALTH_TYPE_GUN_NULL)->Render(1, x, y + ARTICULAR_GUNPOWER_HEALTHBAR_Y);
	UIanimationSet->at(HEALTH_TYPE_PLAYER_NULL)->Render(1, x, y + ARTICULAR_PLAYER_HEALTHBAR_Y);
	gunHB->Render();
	playerHB->Render();
}

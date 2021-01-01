#pragma once
#include "Entity.h"
#include "HealthBar.h"
#include "Jason.h"
#include "Player.h"
using namespace std;

//#define MAX_TEXTLENGHT_SCORE					6
//#define MAX_TEXTLENGHT_REMAININGTIME			4
//#define MAX_TEXTLENGHT_STAGE					2
//#define MAX_TEXTLENGHT_MANA						2
//#define MAX_TEXTLENGHT_LIVE						2

#define ARTICULAR_GUNPOWER_HEALTHBAR_Y				5
#define ARTICULAR_PLAYER_HEALTHBAR_Y				110

class HUD
{
private:
	static HUD* instance;

	float x, y;

	LPANIMATION_SET animationSet;

	HealthBar* playerHB;
	HealthBar* gunHB;

public:
	static HUD* GetInstance();
	HUD() {};
	~HUD();
	void HUDInit(int initPlayerHealth = 8, int initGunHealth = 8);
	void Update(float x, float y, EntityType playerType, int currentPlayerHealth, int currentGunHealth=0);
	void Render(LPGAMEPLAYER playerInfo);
};
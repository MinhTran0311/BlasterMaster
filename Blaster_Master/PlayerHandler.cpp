#include "PlayerHandler.h"
PlayerHandler::PlayerHandler()
{
}

PlayerHandler::~PlayerHandler()
{
}

PlayerHandler* PlayerHandler::instance = NULL;
PlayerHandler* PlayerHandler::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new PlayerHandler();
	}
	return instance;
}

void PlayerHandler::SetJasonInfor(int stage, float xPos, float yPos, int health, int dam, int direction)
{
	SetJasonStage(stage);
	SetJasonPosition(xPos, yPos);
	SetJasonHealth(health);
	SetJasonGunDam(dam);
	if (direction!=0)
		SetPlayerDirectionBeforePassGate(direction);
}
void PlayerHandler::SetSophiaInfor(int stage, float xPos, float yPos, int health, int dam, int direction)
{
	SetSophiaStage(stage);
	SetSophiaPosition(xPos, yPos);
	SetSophiaHealth(health);
	SetSophiaGunDam(dam);
	if (direction != 0)
		SetPlayerDirectionBeforePassGate(direction);
}

void PlayerHandler::Restart()
{
	life = 2;
	playerDirectionBeforePassGate = 1;
	jasonHealth = PLAYER_MAX_HEALTH;
	jasonGundam = PLAYER_DEFAULT_GUNDAM;
	jasonStage = 1;
	jasonXPos, jasonYPos;
	sophiaHealth = PLAYER_MAX_HEALTH;
	sophiaGundam = PLAYER_DEFAULT_GUNDAM;
	sophiaStage = 1;
	sophiaXPos, sophiaYPos;
	specialWeaponId = 1;

	noOfHomingMisslesWeaponLeft = 3;
	noOfElectricWeaponLeft = 3;
	noOfRocketsWeaponLeft = 3;
}

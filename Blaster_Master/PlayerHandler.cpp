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

void PlayerHandler::Init()
{
	life = 2;
	playerDirectionBeforePassGate = 1;
	jasonHealth = PLAYER_MAX_HEALTH;
	jasonGundam = 8;
	jasonStage = 1;
	jasonXPos = 55;
	jasonYPos=100;
	sophiaHealth = PLAYER_MAX_HEALTH;
	sophiaGundam = PLAYER_DEFAULT_GUNDAM;
	sophiaStage = 1;
	sophiaXPos, sophiaYPos;
	specialWeaponId = 1;
	isWinBoss = false;
	noOfHomingMisslesWeaponLeft = 3;
	noOfElectricWeaponLeft = 3;
	noOfRocketsWeaponLeft = 3;
}
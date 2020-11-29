#include "Player.h"
Player::Player()
{
	
}

Player::~Player()
{
}
Player* Player::instance = NULL;
Player* Player::GetInstance()
{
	if (instance == NULL)
		instance = new Player();
	return instance;
}

void Player::SetInjured(int dame)
{
	canChangeAlpha = true;
	if (isImmortaling)
		return;
	health -= dame;
	dam -= dame;

	StartUntouchable();
	immortalTimer->Start();
	isImmortaling = true;
}

void Player::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
#pragma region Death or not
	if (isDoneDeath)
		return;
	if (health <= 0)
	{
		isDeath = true;
		vx = 0;
		vy = 0;
	}
#pragma endregion
	Entity::Update(dt);
}

#include "Player.h"
#include <algorithm>
#include <assert.h>
#include "debug.h"
#include "Game.h"
#include "Worms.h"
#include "Gate.h"
#include "PlayScene.h"
#include "Grid.h"
#include "JasonBullet.h"
#include "JasonRocket.h"
#include "InjuringBrick.h"
#include "GadBrick.h"
#include "ElectricBullet.h"
#include "HomingMissles.h"
Player::Player()
{
	DebugOut(L"Lop cha");
	tag = EntityType::TAG_PLAYER;
	alpha = 255;
	bbARGB = 250;
	canChangeAlpha = true;
	canFire = true;
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

void Player::changeAlpha()
{
	if (canChangeAlpha==false)
		return;
	if (alpha == 255)
		alpha = 140;
	else alpha = 255;
	changeAlphaTimer->Start();
	canChangeAlpha = false;
}

void Player::SetInjured(int dame)
{
	
	if (isImmortaling)
		return;
	health -= dame;
	dam -= dame;

	StartUntouchable();
	immortalTimer->Start();
	isImmortaling = true;
}

void Player::Reset(int initHealth, int initGundam)
{
	health = initHealth;
	dam = initGundam;
	isDoneDeath = false;
	isDeath = false;
	SetState(0);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
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
	if (isImmortaling && immortalTimer->IsTimeUp())
	{
		isImmortaling = false;
		immortalTimer->Reset();
	}
#pragma endregion
	Entity::Update(dt);
	if (!canFire && FireTimer->IsTimeUp())
	{
		canFire = true;
		FireTimer->Reset();
	}

	if (!canChangeAlpha && changeAlphaTimer->IsTimeUp())
	{
		changeAlphaTimer->Reset();
		canChangeAlpha = true;
		if (!isImmortaling)
			alpha = 255;
	}
}

void Player::CollideWithObject(LPGAMEENTITY object,bool &isInjured)
{
	if (this->IsCollidingObject(object))
	{
		switch (object->GetType())
		{
		case EntityType::ENEMY:
		{
			Enemy* enemy = dynamic_cast<Enemy*>(object);
			//re check
			//if (isJumping)
			//{
			//	this->SetState(SOPHIA_STATE_IDLE);
			//	isJumping = false;
			//	isJumpHandle = true;
			//}
			this->changeAlpha();
			DebugOut(L"alpha %d\n", alpha);
			isInjured = true;
			SetInjured(enemy->GetDamage());
			break;
		}
		case EntityType::ITEM:
		{
			LPGAMEITEM item = dynamic_cast<LPGAMEITEM>(object);
			if (item->getItemType() == EntityType::TAG_ITEM_POWER_UP)
			{
				if (this->GetHealth() + ITEM_POWER_UP_RESTORE <= MAX_HEALTH)
					this->AddHealth(ITEM_POWER_UP_RESTORE);
				else
					this->SetHealth(MAX_HEALTH);
			}
			else if (item->getItemType() == EntityType::TAG_ITEM_GUN_UP)
			{
				if (this->GetgunDam() + ITEM_GUN_UP_RESTORE <= MAX_GUNDAM)
					this->AddgunDam(ITEM_GUN_UP_RESTORE);
				else
					this->SetgunDam(MAX_GUNDAM);
			}
			item->setActive(false);
			break;
		}
		case EntityType::TAG_INJURING_BRICK:
		{
			InjuringBrick* injuringBricks = dynamic_cast<InjuringBrick*>(object);
			SetInjured(injuringBricks->GetDamage());
			this->changeAlpha();
			isInjured = true;
			break;
		}
		}
	}
}

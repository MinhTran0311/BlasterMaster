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
#include "Big_Sophia.h"
Player::Player()
{
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
	Sound::GetInstance()->Play("PlayerInjured", 0, 1);
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
			Sound::GetInstance()->Play("PickingItems", 0, 1);
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
void Player::CollisionHandle(vector<LPGAMEENTITY>* coObjects)
{
	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	bool isInjured = false;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		CollideWithObject(coObjects->at(i), isInjured);
		if ((coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_GATE_OVERWORLD || coObjects->at(i)->GetType() == TAG_SOFT_BRICK || coObjects->at(i)->GetType() == TAG_GATE) && !coObjects->at(i)->IsDeath())
			colliable_Objects->push_back(coObjects->at(i));
	}
	if (!isInjured)
		alpha = 255;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (state != PLAYER_STATE_DIE)
		CalcPotentialCollisions(colliable_Objects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == EntityType::TAG_BRICK || e->obj->GetType() == EntityType::TAG_SOFT_BRICK)
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (e->ny != 0)
				{
					if (e->ny != 0)
					{
						vy = 0;
						if (this->GetPlayerType() == TAG_JASON)
						{
							dynamic_cast<JASON*>(this)->SetIsJumping(false);
						}
						else if (this->GetPlayerType() == TAG_SMALL_SOPHIA)
							dynamic_cast<Small_Sophia*>(this)->SetIsJumping(false);
					}

					if (e->nx != 0)
						vx = 0;
				}
			}
			else if (e->obj->GetType() == EntityType::TAG_GATE_OVERWORLD || e->obj->GetType() == EntityType::TAG_GATE)
			{
				gate = e->obj;
				DebugOut(L"big sophia dung tuong\n");
				GateColliding = true;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

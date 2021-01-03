#include "Player.h"
#include <algorithm>
#include <assert.h>
#include "debug.h"
#include "Game.h"
#include "Gate.h"
#include "PlayScene.h"
#include "Grid.h"
#include "JasonBullet.h"
#include "JasonRocket.h"
#include "InjuringBrick.h"
#include "ElectricBullet.h"
#include "HomingMissles.h"
#include "Big_Sophia.h"
#include "Item.h"
#include "PlayerHandler.h"
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

	StartUntouchable();
	immortalTimer->Start();
	isImmortaling = true;
}

void Player::Reset(int initHealth, int initGundam)
{
	health = initHealth;
	dam = initGundam;
	isDoneDeathAni = false;
	isDeath = false;
	SetState(0);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void Player::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
#pragma region Death or not
	if (isDoneDeathAni)
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
		isInjured = false;
		if (!isImmortaling)
			alpha = 255;
	}
}

void Player::EnemyBulletHitPlayer(int dam)
{
	isInjured = true;
	changeAlpha();
	SetInjured(dam);
}

void Player::CollideWithObject(LPGAMEENTITY object)
{
	if (this->IsCollidingObject(object))
	{
		switch (object->GetType())
		{
		case EntityType::ENEMY:
		{
			Enemy* enemy = dynamic_cast<Enemy*>(object);
			this->changeAlpha();
			DebugOut(L"alpha %d\n", alpha);
			isInjured = true;
			SetInjured(enemy->GetDamage());
			break;
		}
		case EntityType::ITEM:
		{
			EntityType itemType = dynamic_cast<LPGAMEITEM>(object)->GetItemType();

			switch (itemType)
			{
			case TAG_ITEM_SINGLE_POWER_UP:
			{
				if (this->GetHealth() + ITEM_POWER_UP_RESTORE <= MAX_HEALTH)
					this->AddHealth(ITEM_POWER_UP_RESTORE);
				else
					this->SetHealth(MAX_HEALTH);
				break;
			}
			case TAG_ITEM_HOVER:
				break;
			case TAG_ITEM_ELECTRIC:
			{
				int no = PlayerHandler::GetInstance()->GetNoElectric();
				no++;
				PlayerHandler::GetInstance()->SetNoElectric(no);
				break;
			}
			case TAG_ITEM_ROCKET:
			{
				int no = PlayerHandler::GetInstance()->GetNoRocket();
				no++;
				PlayerHandler::GetInstance()->SetNoRocket(no);
				break;
			}
			case TAG_ITEM_HOMINGMISSLES:
			{
				int no = PlayerHandler::GetInstance()->GetNoMissles();
				no++;
				PlayerHandler::GetInstance()->SetNoMissles(no);
				break;
			}
			case TAG_ITEM_SINGLE_GUN_UP:
			{
				if (this->GetgunDam() + ITEM_GUN_UP_RESTORE <= MAX_GUNDAM)
					this->AddgunDam(ITEM_GUN_UP_RESTORE);
				else
					this->SetgunDam(MAX_GUNDAM);
				break;
			}
			case TAG_ITEM_FULL_GUN_UP:
			{	
				this->SetgunDam(MAX_GUNDAM);
				break;
			}
			case TAG_ITEM_FULL_POWER_UP:
			{
				this->SetHealth(MAX_HEALTH);
				break;
			}
			case TAG_ITEM_CRUSHER_BEAM:
			{
				int jasongundam = PlayerHandler::GetInstance()->GetJasonGunDam();
				jasongundam += 2;
				PlayerHandler::GetInstance()->SetJasonGunDam(jasongundam);
				break;
			}
			case TAG_ITEM_HYBER_BEAM:
			{
				int jasongundam = PlayerHandler::GetInstance()->GetJasonGunDam();
				jasongundam += 1;
				PlayerHandler::GetInstance()->SetJasonGunDam(jasongundam);
				break;
			}
			}
			object->setActive(false);
			isInjured = false;
			Sound::GetInstance()->Play("PickingItems", 0, 1);
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
		default:
			//isInjured = false;
			break;

		}
	}
}
void Player::CollisionHandle(vector<LPGAMEENTITY>* coObjects)
{
	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	//bool isInjured = false;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		CollideWithObject(coObjects->at(i));

		if (GetPlayerType() == TAG_SMALL_SOPHIA && coObjects->at(i)->GetType() == TAG_STAIRS)
		{
			if (dynamic_cast<Small_Sophia*>(this)->IsCollidingObject(coObjects->at(i)))
			{
				DebugOut(L"Dinh cau thang\n");
				dynamic_cast<Small_Sophia*>(this)->SetIsInStairs(true);

			}
			else
			{
				DebugOut(L"Khong dung\n");
				dynamic_cast<Small_Sophia*>(this)->SetIsInStairs(false);
			}
		}

		if ((coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_GATE_OVERWORLD || coObjects->at(i)->GetType() == TAG_SOFT_BRICK || coObjects->at(i)->GetType() == TAG_GATE) && !coObjects->at(i)->IsDeath())
			colliable_Objects->push_back(coObjects->at(i));
	}
	if (!isInjured)
		alpha = 255;
	gate = nullptr;
	GateColliding = false;
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
					if (e->ny == -1)
					{
						vy = 0;

						if (this->GetPlayerType() == TAG_JASON)
						{
							dynamic_cast<JASON*>(this)->SetIsJumping(false);
						}
						else if (this->GetPlayerType() == TAG_SMALL_SOPHIA)
							dynamic_cast<Small_Sophia*>(this)->SetIsJumping(false);
					}
					if (e->ny == 1)
					{
						if (this->GetPlayerType() == TAG_JASON)
							dynamic_cast<JASON*>(this)->SetIsJumping(true);
						else if (this->GetPlayerType() == TAG_SMALL_SOPHIA)
							dynamic_cast<Small_Sophia*>(this)->SetIsJumping(true);
						vy = 0;
					}
					if (e->nx != 0)
						vx = 0;
				}
				

			}
			else if (e->obj->GetType() == EntityType::TAG_GATE_OVERWORLD || e->obj->GetType() == EntityType::TAG_GATE)
			{
				gate = e->obj;
				GateColliding = true;
			}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

#include "Jason.h"
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

JASON::JASON(float x, float y, int _health, int _gundam)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_JASON));
	SetState(SOPHIA_STATE_IDLE);
	_PlayerType = EntityType::TAG_JASON;
	tag = TAG_PLAYER;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	current_Jumpy = 0;
	isImmortaling = false;
	canFire = true;
	isDeath = false;
	alpha = 255;
	bbARGB = 250;
	health = _health;
	dam = _gundam;
	canChangeAlpha = false;
}

JASON* JASON::instance = NULL;
JASON* JASON::GetInstance()
{
	if (instance == NULL)
		instance = new JASON();
	return instance;
}

void JASON::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case SOPHIA_STATE_WALKING_RIGHT:
		vx = SOPHIA_WALKING_SPEED;
		nx = 1;
		break;
	case SOPHIA_STATE_WALKING_LEFT:
		vx = -SOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	case SOPHIA_STATE_JUMP:
		isPressJump = true;
		if (isJumping == true)
			return;
		else
		{
			isJumpHandle = false;
			isJumping = true;
			vy = -SOPHIA_JUMP_SPEED_Y;
			current_Jumpy = y;
		}
		break;
	case SOPHIA_STATE_IDLE:
		isPressJump = false;
		//isJumpHandle = true;
		if (vx > 0) {
			vx -= SOPHIA_WALKING_ACC * dt;
			if (vx < 0)
				vx = 0;
		}
		else if (vx < 0)
		{
			vx += SOPHIA_WALKING_ACC * dt;
			if (vx > 0)
				vx = 0;
		}
		break;
	case SOPHIA_STATE_OUT:
		vx = 0;
		isEjecting = true;
		break;
	}
}

void JASON::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Player::Update(dt, coObjects);
	//fall down
#pragma region fall down
	vy += SOPHIA_GRAVITY * dt;
	//check player's height
	if (isJumping && current_Jumpy - y >= HEIGHT_LEVER1 && isJumpHandle == false)
	{
		if (!isPressJump)
			vy = 0;
		isJumpHandle = true;
	}
#pragma endregion
#pragma region gun flip
	if (isPressFlipGun == false)
	{
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_1)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_2)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_3)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_4)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_1)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_2)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_3)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_4)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_RIGHT)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_LEFT)->ResetCurrentFrame();
	}

#pragma endregion
#pragma region Timer
	if (isImmortaling && immortalTimer->IsTimeUp())
	{
		canChangeAlpha = false;
		isImmortaling = false;
		immortalTimer->Reset();
		//changeAlphaTimer->Reset();
	}

	if (!canFire && FireTimer->IsTimeUp())
	{
		canFire = true;
		FireTimer->Reset();
	}
	if (canChangeAlpha)
	{
		if (alpha == 255)
			alpha = 140;
		else alpha = 255;
	}
#pragma endregion
#pragma region Collision
	//ABBA with objects
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (this->IsCollidingObject(coObjects->at(i)))
		{
			switch (coObjects->at(i)->GetType())
			{

			case EntityType::ENEMY:
			{
				Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));
				//re check
				if (isJumping)
				{
					this->SetState(SOPHIA_STATE_IDLE);
					isJumping = false;
					isJumpHandle = true;
				}
				SetInjured(enemy->GetDamage());
				break;
			}
			case EntityType::ITEM:
			{
				LPGAMEITEM item = dynamic_cast<LPGAMEITEM>(coObjects->at(i));
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
				InjuringBrick* injuringBricks = dynamic_cast<InjuringBrick*>(coObjects->at(i));
				SetInjured(injuringBricks->GetDamage());
				break;
			}
			}
		}
	}

	//filter brick and gate object before collision handle
	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();

	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK || coObjects->at(i)->GetType() == EntityType::TAG_GATE || coObjects->at(i)->GetType() == EntityType::TAG_GAD_BRICK || coObjects->at(i)->GetType() == EntityType::TAG_SOFT_BRICK)
			colliable_Objects->push_back(coObjects->at(i));
	}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		// turn off collision when player dies
		if (state != SOPHIA_STATE_DIE)
			CalcPotentialCollisions(colliable_Objects, coEvents);

		// No collision occured, proceed normally
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
				if (e->obj->GetType() == TAG_BRICK || e->obj->GetType() == TAG_SOFT_BRICK)
				{
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					if (e->ny != 0)
					{
						if (e->ny != 0)
						{
							vy = 0;
							if (ny < 0)
								isJumping = false;
						}
						if (e->nx != 0)
						{
							vx = 0;
						}
					}
				}
				else if ((e->obj->GetType() == EntityType::TAG_GATE))
				{
					gate = dynamic_cast<Gate*>(e->obj);
					DebugOut(L"jason dung tuong loai 1");
					GateColliding = true;
				}
				else if ((e->obj->GetType() == EntityType::TAG_GAD_BRICK))
				{
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					if (e->ny != 0)
					{
						if (e->ny != 0)
						{
							vy = 0;
							if (ny < 0)
								isJumping = false;
						}
						if (e->nx != 0)
						{
							vx = 0;
						}
					}
					//SetInjured(dynamic_cast<GadBrick*>(e->obj)->GetDamage());
					health--;
				}
			}
		}
		//khi va cham chua xet gia tri x va y
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void JASON::Render()
{
	if (isDoneDeath)
		return;
	//RenderBoundingBox();

	int ani = -1;
	int current_frame;

	if (isDeath)
	{
		ani = SOPHIA_JASON_ANI_DIE;
		animationSet->at(ani)->Render(nx, x - DURATION_X_TO_DIE, y - DURATION_Y_TO_DIE, alpha);
		if (animationSet->at(ani)->GetFrame() > 1)
		{
			isDoneDeath = true;
		}
	}
	else if (isEjecting)
	{
		ani = SOPHIA_JASON_ANI_EJECTING;
		animationSet->at(ani)->Render(nx, x, y - 8, alpha);
		if (animationSet->at(ani)->GetFrame() >= 1)
		{
			isEjecting = false;
		}
	}
	else if (isPressFlipGun == true)
	{
		if (vx == 0)
		{
			if (nx > 0)
			{
				//idle theo walking
				current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrameStopWalking();
				switch (current_frame)
				{
				case SOPHIA_STOP_WALKING_SPRITE2:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_2;
					break;
				case SOPHIA_STOP_WALKING_SPRITE3:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_3;
					break;
				case SOPHIA_STOP_WALKING_SPRITE4:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_4;
					break;
				default:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_1;
					break;
				}
			}
			else
			{
				current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_LEFT)->GetFrameStopWalking();
				switch (current_frame)
				{
				case SOPHIA_STOP_WALKING_SPRITE2:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_2;
					break;
				case SOPHIA_STOP_WALKING_SPRITE3:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_3;
					break;
				case SOPHIA_STOP_WALKING_SPRITE4:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_4;
					break;
				default:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_1;
					break;
				}
			}

			if (isGunFlipping == false)
			{
				animationSet->at(ani)->OldRender(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
				if (animationSet->at(ani)->GetFrame() == 1)
				{
					//DebugOut(L"[frame]: %d;\n", animation_set->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrame());
					isGunFlipping = true;
				}
				return;

			}
			else
			{
				animationSet->at(ani)->RenderFrame(1, x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
				return;
			}
		}
		else if (vx > 0)
			ani = SOPHIA_ANI_GUN_FLIP_RIGHT;
		else
			ani = SOPHIA_ANI_GUN_FLIP_LEFT;

		if (isGunFlipping == false)
		{
			animationSet->at(ani)->RenderGunFlip(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
			if (animationSet->at(ani)->GetFrame() > 3)
			{
				//DebugOut(L"[frame]: %d;\n", animation_set->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrame());
				isGunFlipping = true;
			}
			return;
		}
		else
		{
			animationSet->at(ani)->RenderGunFlipTargetTop(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
			return;
		}
	}
	else
	{
		if (isJumping == false)
		{
			if (vx == 0)
			{
				if (nx > 0)
				{
					ani = SOPHIA_ANI_JASON_WALKING_RIGHT;
					current_frame = animationSet->at(ani)->GetFrameStopWalking();
				}
				else
				{
					ani = SOPHIA_ANI_JASON_WALKING_LEFT;
					current_frame = animationSet->at(ani)->GetFrameStopWalking();
				}
				animationSet->at(ani)->RenderFrame(current_frame, x, y, alpha);
				isGunFlipping = false;
				return;
			}
			else if (vx > 0)
				ani = SOPHIA_ANI_JASON_WALKING_RIGHT;
			else ani = SOPHIA_ANI_JASON_WALKING_LEFT;
			animationSet->at(ani)->OldRender(x, y, alpha);
			isGunFlipping = false;
		}
		else {
			if (vx == 0 && vy > 0)
			{
				//if (nx > 0) ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT;
				//else ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT;
				if (nx > 0)
				{
					//idle theo walking
					current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrameStopWalking();
					switch (current_frame)
					{
					case SOPHIA_STOP_WALKING_SPRITE2:
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT_2;
						break;
					case SOPHIA_STOP_WALKING_SPRITE3:
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT_3;
						break;
					case SOPHIA_STOP_WALKING_SPRITE4:
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT_4;
						break;
					default:
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT_1;
						break;
					}
				}
				else
				{
					current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_LEFT)->GetFrameStopWalking();
					switch (current_frame)
					{
					case SOPHIA_STOP_WALKING_SPRITE2:
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT_2;
						break;
					case SOPHIA_STOP_WALKING_SPRITE3:
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT_3;
						break;
					case SOPHIA_STOP_WALKING_SPRITE4:
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT_4;
						break;
					default:
						ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT_1;
						break;
					}
				}
			}
			else if (vx == 0 && vy <= 0)
			{
				//if (nx > 0) ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT;
				//else ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT;
				if (nx > 0)
				{
					//idle theo walking
					current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrameStopWalking();
					switch (current_frame)
					{
					case SOPHIA_STOP_WALKING_SPRITE2:
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT_2;
						break;
					case SOPHIA_STOP_WALKING_SPRITE3:
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT_3;
						break;
					case SOPHIA_STOP_WALKING_SPRITE4:
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT_4;
						break;
					default:
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT_1;
						break;
					}
				}
				else
				{
					current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_LEFT)->GetFrameStopWalking();
					switch (current_frame)
					{
					case SOPHIA_STOP_WALKING_SPRITE2:
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT_2;
						break;
					case SOPHIA_STOP_WALKING_SPRITE3:
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT_3;
						break;
					case SOPHIA_STOP_WALKING_SPRITE4:
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT_4;
						break;
					default:
						ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT_1;
						break;
					}
				}
			}
			else if (vy > 0)
			{
				if (vx > 0) ani = SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_LEFT;
			}
			else if (vy <= 0)
			{
				if (vx > 0) ani = SOPHIA_ANI_JASON_JUMP_UP_WALKING_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_UP_WALKING_LEFT;
			}
			isGunFlipping = false;
			animationSet->at(ani)->OldRender(x, y, alpha);
			return;
		}
	}

}

void JASON::Reset()
{
	//health = MAX_HEALTH;
	isDoneDeath = false;
	isDeath = false;
	SetState(SOPHIA_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

//void JASON::SetInjured(int dame)
//{
//	canChangeAlpha = true;
//	if (isImmortaling)
//		return;
//	health -= dame;
//	dam -= dame;
//
//	StartUntouchable();
//	immortalTimer->Start();
//	isImmortaling = true;
//}

void JASON::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (isDoneDeath == false)
	{
		left = x;
		top = y;
		right = x + SOPHIA_JASON_BBOX_WIDTH;
		bottom = y + SOPHIA_JASON_BBOX_HEIGHT;
	}
}

void JASON::FireBullet(int type)
{
	if (!canFire)
		return;

	if (CGrid::GetInstance()->CheckBulletLimitation(JASON_NORMAL_BULLET, this->Getx(), this->Gety(), 3))
	{
		Bullet* bullet = new JasonBullet(this->Getx(), this->Gety(), type, nx, isGunFlipping);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
	FireTimer->Start();
	canFire = false;
}

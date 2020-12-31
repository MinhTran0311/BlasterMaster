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
#include "ElectricBullet.h"
#include "HomingMissles.h"

JASON::JASON(float x, float y, int _health, int _gundam)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_JASON));
	SetState(SOPHIA_STATE_IDLE);
	_PlayerType = EntityType::TAG_JASON;
	current_Jumpy = 0;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	health = _health;
	dam = _gundam;
	specialBulletType = JASON_HOMING_MISSLES;
	//isJumping = true;
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
			Sound::GetInstance()->Play("Jump", 0, 1);
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
	if (vy > FALLING_VELOCITY_UPPER_LIMITATION) isJumping = true;
	vy += SOPHIA_GRAVITY *dt;
	//check player's height
	if (isJumping && current_Jumpy - y >= HEIGHT_LEVER1 && isJumpHandle == false)
	{
		if (!isPressJump)
			vy = 0;
		isJumpHandle = true;
	}
	//DebugOut(L"vy: %f\n", vy);

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
	if (FireTimer->IsTimeUp() && burstFireModeBullets > 0)
	{
		if (dam == 1)
		{
			Bullet* bullet1 = new JasonBullet(this->Getx(), this->Gety(), 0, nx, isGunFlipping);
			CGrid::GetInstance()->InsertGrid(bullet1);
		}
		else {

			Bullet* bullet1 = new JasonBullet(this->Getx(), this->Gety(), 1, nx, isGunFlipping);
			CGrid::GetInstance()->InsertGrid(bullet1);
		}
		burstFireModeBullets--;
		Sound::GetInstance()->Play("PlayerFireUnderWorld", 0, 1);
		FireTimer->Start();
		canFire = false;
	}
#pragma region Collision
	CollisionHandle(coObjects);
	//vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	//bool isInjured = false;
	////ABBA with objects
	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	//if (this->IsCollidingObject(coObjects->at(i)))
	//	//{
	//	//	switch (coObjects->at(i)->GetType())
	//	//	{
	//	//	case EntityType::ENEMY:
	//	//	{
	//	//		Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));
	//	//		//re check
	//	//		//if (isJumping)
	//	//		//{
	//	//		//	this->SetState(SOPHIA_STATE_IDLE);
	//	//		//	isJumping = false;
	//	//		//	isJumpHandle = true;
	//	//		//}
	//	//		this->changeAlpha();
	//	//		DebugOut(L"alpha %d\n", alpha);
	//	//		isInjured = true;
	//	//		SetInjured(enemy->GetDamage());
	//	//		break;
	//	//	}
	//	//	case EntityType::ITEM:
	//	//	{
	//	//		
	//	//		LPGAMEITEM item = dynamic_cast<LPGAMEITEM>(coObjects->at(i));
	//	//		if (item->getItemType() == EntityType::TAG_ITEM_POWER_UP)
	//	//		{
	//	//			if (this->GetHealth() + ITEM_POWER_UP_RESTORE <= MAX_HEALTH)
	//	//				this->AddHealth(ITEM_POWER_UP_RESTORE);
	//	//			else
	//	//				this->SetHealth(MAX_HEALTH);
	//	//		}
	//	//		else if (item->getItemType() == EntityType::TAG_ITEM_GUN_UP)
	//	//		{
	//	//			if (this->GetgunDam() + ITEM_GUN_UP_RESTORE <= MAX_GUNDAM)
	//	//				this->AddgunDam(ITEM_GUN_UP_RESTORE);
	//	//			else
	//	//				this->SetgunDam(MAX_GUNDAM);
	//	//		}
	//	//		item->setActive(false);
	//	//		break;
	//	//	}
	//	//	case EntityType::TAG_INJURING_BRICK:
	//	//	{
	//	//		InjuringBrick* injuringBricks = dynamic_cast<InjuringBrick*>(coObjects->at(i));
	//	//		SetInjured(injuringBricks->GetDamage());
	//	//		this->changeAlpha();
	//	//		isInjured = true;
	//	//		break;
	//	//	}
	//	//	}
	//	//}
	//	CollideWithObject(coObjects->at(i), isInjured);
	//	if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK || coObjects->at(i)->GetType() == EntityType::TAG_GATE || coObjects->at(i)->GetType() == EntityType::TAG_SOFT_BRICK)
	//		colliable_Objects->push_back(coObjects->at(i));
	//}
	//if (!isInjured)
	//	alpha = 255;
	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;
	//coEvents.clear();
	//// turn off collision when player dies
	//if (state != SOPHIA_STATE_DIE)
	//	CalcPotentialCollisions(colliable_Objects, coEvents);
	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];
	//		if (e->obj->GetType() == TAG_BRICK || e->obj->GetType() == TAG_SOFT_BRICK)
	//		{
	//			x += min_tx * dx + nx * 0.4f;
	//			y += min_ty * dy + ny * 0.4f;
	//			if (e->ny != 0)
	//			{
	//				if (e->ny != 0)
	//				{
	//					vy = 0;
	//					if (ny < 0)
	//					{
	//						isJumping = false;
	//					}
	//				}
	//				if (e->nx != 0)
	//				{
	//					vx = 0;
	//				}
	//			}
	//		}
	//		else if ((e->obj->GetType() == EntityType::TAG_GATE))
	//		{
	//			gate = e->obj;
	//			DebugOut(L"jason dung tuong loai 1\n");
	//			GateColliding = true;
	//		}
	//	}
	//}
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
		if (animationSet->at(ani)->GetFrame() == animationSet->at(ani)->GetLastFrameIndex())
		{
			isDoneDeath = true;
			animationSet->at(ani)->ResetCurrentFrame();
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

void JASON::FireBullet(int mode)
{
	if (!canFire || burstFireModeBullets>0)
		return;
	if (mode == 1)
	{
		if (dam == 1)
		{
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_NORMAL_BULLET, this->Getx(), this->Gety(), 3))
			{
				Bullet* bullet = new JasonBullet(this->Getx(), this->Gety(), 0 , nx, isGunFlipping);
				Sound::GetInstance()->Play("PlayerFireUnderWorld", 0, 1);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			FireTimer->Start();
			canFire = false;
		}
		else
		{
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_UPGRADE_BULLET, this->Getx(), this->Gety(), 3))
			{
				Bullet* bullet = new JasonBullet(this->Getx(), this->Gety(), 1, nx, isGunFlipping);
				Sound::GetInstance()->Play("PlayerFireUnderWorld", 0, 1);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			FireTimer->Start();
			canFire = false;
		}

	}
	else if (mode == 2)		//burst fire
	{
		if (dam == 1)
		{
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_NORMAL_BULLET, this->Getx(), this->Gety(), 3))
			{
				Bullet* bullet1 = new JasonBullet(this->Getx(), this->Gety(), 0, nx, isGunFlipping);
				Sound::GetInstance()->Play("PlayerFireUnderWorld", 0, 1);
				CGrid::GetInstance()->InsertGrid(bullet1);
				burstFireModeBullets = 3-CGrid::GetInstance()->GetNumberOfBulletInGrid(JASON_NORMAL_BULLET, this->Getx(), this->Gety());
			}
			FireTimer->Start();
			canFire = false;
		}
		else
		{
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_UPGRADE_BULLET, this->Getx(), this->Gety(), 3))
			{
				Bullet* bullet1 = new JasonBullet(this->Getx(), this->Gety(), 1, nx, isGunFlipping);
				Sound::GetInstance()->Play("PlayerFireUnderWorld", 0, 1);
				CGrid::GetInstance()->InsertGrid(bullet1);
				burstFireModeBullets = 3-CGrid::GetInstance()->GetNumberOfBulletInGrid(JASON_UPGRADE_BULLET, this->Getx(), this->Gety());
			}
			FireTimer->Start();
			canFire = false;
		}
	}
	else if (mode == 3) //special bullets
	{
		switch (specialBulletType)
		{
		case JASON_ROCKET_BULLET:
		{
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_ROCKET_BULLET, this->Getx(), this->Gety(), 2) && noOfRocketsWeaponLeft>0)
			{
				Bullet* rocket = new JasonRocket(this->Getx(), this->Gety(), nx);
				CGrid::GetInstance()->InsertGrid(rocket);
				Sound::GetInstance()->Play("FireRocket", 0, 1);
				noOfRocketsWeaponLeft--;
			}

			break;
		}
		case JASON_ELECTRIC_BULLET:
		{
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_ELECTRIC_BULLET, this->Getx(), this->Gety(), 1) && noOfElectricWeaponLeft>0)
			{
				Bullet* electric = new ElectricBullet(this->Getx(), this->Gety());
				CGrid::GetInstance()->InsertGrid(electric);
				noOfElectricWeaponLeft--;
				Sound::GetInstance()->Play("FireElectricBullet", 0, 1);
			}
			break;
		}
		case JASON_HOMING_MISSLES:
			if (CGrid::GetInstance()->CheckBulletLimitation(JASON_HOMING_MISSLES, this->Getx(), this->Gety(), 1) && noOfHomingMisslesWeaponLeft>0)
			{
				Bullet* homingMissles = new HomingMissles(this->Getx(), this->Gety(), nx);
				CGrid::GetInstance()->InsertGrid(homingMissles);
				Sound::GetInstance()->Play("FireHomingMissles", 0, 1);
				noOfHomingMisslesWeaponLeft--;
			}

			break;
		}

		FireTimer->Start();
		canFire = false;
	}
}

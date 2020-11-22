#include "Cannons.h"
#include <time.h>

void Cannons::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CANNONS_BBOX_WIDTH;
	bottom = y + CANNONS_BBOX_HEIGHT;
}

void Cannons::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	bbARGB = 0;

	//#pragma region fall down
#pragma endregion
#pragma region Pre-collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;

	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			bricks.push_back(coObjects->at(i));

		// turn off collision when die 
		if (state != CANNONS_STATE_DIE)
			CalcPotentialCollisions(&bricks, coEvents);
	}
#pragma endregion

#pragma region coillision
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
#pragma region Active
	//attack player
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= CANNONS_SITEATTACK_PLAYER)
	{
		isActive = true;
		AttackTarget(target);
	}
	else
	{
		isActive = false;
		this->SetState(CANNONS_STATE_IDLE);
	}
#pragma endregion

}

void Cannons::Render()
{
	RenderBoundingBox();

	int ani = -1;
	if (state == CANNONS_STATE_ATTACKING_LEFT)
	{
		ani = CANNONS_ANI_ATTACKING_LEFT;
		if (animationSet->at(ani)->GetFrame() == 1)
		{
			isAttackTop = true;
		}
	}
	else if (state == CANNONS_STATE_ATTACKING_TOP)
	{
		ani = CANNONS_ANI_ATTACKING_TOP;
		if (animationSet->at(ani)->GetFrame() == 1)
		{
			isAttackTop = false;
		}
	}
	else if (state == CANNONS_STATE_IDLE)
	{
		ani = CANNONS_ANI_IDLE;
	}
	
	animationSet->at(ani)->OldRender(x, y);
}

Cannons::Cannons(float x, float y, LPGAMEENTITY t)
{
	this->SetState(CANNONS_STATE_IDLE);
	enemyType = CANNONS;
	tag = TAG_CANNONS;
	this->x = x;
	this->y = y;
	dam = 1;
	isAttack = 0;
	this->target = t;
	health = CANNONS_MAXHEALTH;
	isActive = false;
	bbARGB = 250;
	isAttackTop = false;
}

void Cannons::AttackTarget(LPGAMEENTITY target) //tấn công nhân vật
{
	if (isAttackTop)
		SetState(CANNONS_STATE_ATTACKING_TOP);
	else SetState(CANNONS_STATE_ATTACKING_LEFT);
}

void Cannons::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
		case CANNONS_STATE_DIE:
		{
			y += CANNONS_BBOX_HEIGHT - CANNONS_BBOX_HEIGHT_DIE + 1;
			break;
		}
		case CANNONS_STATE_ATTACKING_TOP:
		{
			//Attack

			break;
		}
		case CANNONS_STATE_ATTACKING_LEFT:
		{
			//Attackz

			break;
		}
		case CANNONS_STATE_IDLE:
		{
			break;
		}
	}
}

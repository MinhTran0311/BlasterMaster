﻿#include "Worms.h"

void Worm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WORM_BBOX_WIDTH;
	bottom = y + WORM_BBOX_HEIGHT;
}

void Worm::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(this->target->x, this->target->y)) <= WORM_SITEFOLLOW_PLAYER)// Kiểm tra bán kính xung quanh Worm xem có player không
	{
		isActive = true;
		FollowTarget(this->target);
	}
	vy += WORM_GRAVITY * dt;

#pragma region Active
	if (!isActive) vx = 0;
	else SetState(WORM_STATE_WALKING);
#pragma endregion
	Entity::Update(dt);
}

void Worm::Render()
{
	if (vx > 0)
		nx = 1;
	else
		nx = -1;

	int ani = WORM_ANI_WALKING;
	if (state == WORM_STATE_DIE) {
		ani = WORM_ANI_DIE;
	}
	animationSet->at(ani)->Render(nx, x, y);
	//RenderBoundingBox();
}

Worm::Worm(float x, float y, LPGAMEENTITY t)
{
	SetState(WORM_STATE_WALKING);
	enemyType = EnemyType::WORM;
	this->x = x;
	this->y = y;
	nx = -1;
	isFollow = 0;
	this->target = t;
	health = WORM_MAXHEALTH;
	isActive = false;
}

void Worm::FollowTarget(LPGAMEENTITY target) //đi theo nhân vật
{
	if ((target->x - this->x) > 0)
	{
		this->nx = 1;
		vx = WORM_WALKING_SPEED;
	}
	else
	{
		vx = -WORM_WALKING_SPEED;
		this->nx = -1;
	}

}

void Worm::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case WORM_STATE_DIE:
		y += WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case WORM_STATE_WALKING:
		if (nx > 0)
		{
			vx = WORM_WALKING_SPEED;
		}
		else
		{
			vx = -WORM_WALKING_SPEED;
		}
	}
}

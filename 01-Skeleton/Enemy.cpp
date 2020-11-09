#include "Enemy.h"


CEnemy1::CEnemy1(float x, float y)
{
	this->SetPosition(x, y);
	vx = 0.07f;
}

CEnemy1::CEnemy1()
{
	x = 0;
	y = 0;
	vx = 0.07f;
}

void CEnemy1::Update(DWORD dt)
{
	x += vx * dt;
	if ((vx > 0 && x > 290) || (x < 0 && vx < 0)) vx = -vx;
}

void CEnemy1::Render()
{
	int ani;
	if (vx < 0) ani = 0;
	else ani = 1;
	animations[ani]->Render(x, y);
}

CEnemy2::CEnemy2(float x, float y)
{
	this->SetPosition(x, y);
	vx = vy = 0;
	nx = 1;
}

void CEnemy2::Update(DWORD dt)
{
	x += vx * dt;
	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void CEnemy2::Render()
{
	int ani;
	if (vx == 0)
	{
		if (nx > 0) ani = ENEMY_ANI_IDLE_RIGHT;
		else ani = ENEMY_ANI_IDLE_LEFT;
	}
	else if (vx > 0)
		ani = ENEMY_ANI_JUMPING_RIGHT;
	else ani = ENEMY_ANI_JUMPING_LEFT;
	animations[ani]->Render(x, y);
}

void CEnemy2::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ENEMY_STATE_JUMPING_RIGHT:
		vx = ENEMY_JUMPING_SPEED;
		nx = 1;
		break;
	case ENEMY_STATE_JUMPING_LEFT:
		vx = -ENEMY_JUMPING_SPEED;
		nx = -1;
		break;
	case ENEMY_STATE_IDLE:
		vx = 0;
		break;
	}
}

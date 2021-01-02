#include "Bullet.h"
#include "PlayScene.h"
#include "Boss.h"
Bullet::Bullet()
{
	isActive = true;
}

Bullet::~Bullet()
{
}

void Bullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt, coObjects); 
}

void Bullet::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case BULLET_STATE_FLYING:
	{
		isHitBrick = false;
		isHitEnemy = false;
		isActive = true;
		break;
	}
	case BULLET_STATE_HIT_BRICK:
	{
		vx = 0;
		vy = 0;
		isHitBrick = true;
	}
	case BULLET_STATE_ENEMY:
	{
		vx = 0;
		vy = 0;
		isHitEnemy = true;
		break;
	}
	}
}

void Bullet::HandlePlayerBulletCollision(vector<LPGAMEENTITY>* coObjects)
{
	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == TAG_GATE || coObjects->at(i)->GetType() == TAG_GATE_OVERWORLD || coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_SOFT_BRICK || coObjects->at(i)->GetType() == ENEMY)
			colliable_Objects->push_back(coObjects->at(i));
	}

#pragma region collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

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
			if (e->obj->GetType() == TAG_BRICK || e->obj->GetType() == TAG_GATE || e->obj->GetType() == TAG_GATE_OVERWORLD)
			{
				this->SetState(BULLET_STATE_HIT_BRICK);
				Sound::GetInstance()->Play("PlayerBulletHitBrick", 0, 1);

				//x += min_tx * dx + nx * 0.4f;
				//y += min_ty * dy + ny * 0.4f;
			}
			else if (e->obj->GetType() == ENEMY || e->obj->GetType() == TAG_SOFT_BRICK)
			{
				e->obj->AddHealth(-dam);
				//DebugOut(L"xxxxxxxxxxxxxxxxhitEnemy %d\n", e->obj->health);
				this->SetState(BULLET_STATE_ENEMY);
				//x += min_tx * dx + nx * 0.4f;
				//y += min_ty * dy + ny * 0.4f;
				isActive = false;
				if (dynamic_cast<CBoss*>(e->obj)) {
					dynamic_cast<CBoss*>(e->obj)->SetState(BOSS_STATE_INJURED);
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

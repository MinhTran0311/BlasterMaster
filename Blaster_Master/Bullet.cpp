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

	for (UINT i = 0; i < colliable_Objects->size(); i++)
	{
		if (this->IsCollidingObject(colliable_Objects->at(i)))
		{
			LPGAMEENTITY e = colliable_Objects->at(i);
			if (e->GetType() == TAG_BRICK || e->GetType() == TAG_GATE || e->GetType() == TAG_GATE_OVERWORLD)
			{
				if (e->GetType() == TAG_BRICK)
					if (dynamic_cast<Brick*>(e)->IsCanShootThrought())
					{
						x += dx;
						y += dy;
						continue;
					}

				this->SetState(BULLET_STATE_HIT_BRICK);
				Sound::GetInstance()->Play("PlayerBulletHitBrick", 0, 1);
			}
			else if (e->GetType() == ENEMY)
			{
				e->AddHealth(-dam);
				this->SetState(BULLET_STATE_ENEMY);
				isActive = false;
				if (dynamic_cast<CBoss*>(e)) {
					dynamic_cast<CBoss*>(e)->SetState(BOSS_STATE_INJURED);
				}
			}
			else if (e->GetType() == TAG_SOFT_BRICK)
			{
				if (typeBullet == JASON_UPGRADE_BULLET || typeBullet == BIG_SOPHIA_BULLET_LV1 || typeBullet == BIG_SOPHIA_BULLET_LV2 || typeBullet == BIG_SOPHIA_BULLET_LV3)
				{
					e->AddHealth(-dam);
					this->SetState(BULLET_STATE_ENEMY);
					Sound::GetInstance()->Play("PlayerBulletHitBrick", 0, 1);
					isActive = false;
				}
				else {
					this->SetState(BULLET_STATE_HIT_BRICK);
					Sound::GetInstance()->Play("PlayerBulletHitBrick", 0, 1);
				}
			}
		}
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

		//FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	LPCOLLISIONEVENT e = coEventsResult[i];
		//	
		//}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

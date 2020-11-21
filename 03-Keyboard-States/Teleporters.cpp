#include "Teleporters.h"

void Teleporters::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TELEPORTERS_BBOX_WIDTH;
	bottom = y + TELEPORTERS_BBOX_HEIGHT;
}

void Teleporters::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	for (int i = 0; i < listBullet.size(); i++)
		listBullet[i]->Update(dt, coObjects);
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
		if (state != TELEPORTERS_STATE_DIE)
			CalcPotentialCollisions(&bricks, coEvents);
	}
#pragma endregion

#pragma region coillision
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
#pragma region Active
	//attack player
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= TELEPORTERS_SITEATTACK_PLAYER)
	{
		isActive = true;
		//isTransporting = true;
		AttackTarget(target);
	}
	else
	{
		this->SetState(TELEPORTERS_STATE_TRANSFORMING);
		transportTimer->Start();
		if (transportTimer->IsTimeUp()) {
			isTransporting = true;
			this->SetState(TELEPORTERS_STATE_IDLE);
			isActive = false;
			transportTimer->Reset();
		}
	}
#pragma endregion

}

void Teleporters::Render()
{
	RenderBoundingBox();
	for (int i = 0; i < listBullet.size(); i++)
		listBullet[i]->Render();

	int ani = -1;
	if (state == TELEPORTERS_STATE_TRANSFORMING)
	{
		/*if (animationSet->at(ani)->GetFrame() == 3)
		{
			ani = TELEPORTERS_ANI_TRANSFORMING;
		}*/
		ani = TELEPORTERS_ANI_TRANSFORMING;
	}
	else if (state == TELEPORTERS_STATE_ATTACKING)
	{
		ani = TELEPORTERS_ANI_ATTACKING;
	}
	else if (state == TELEPORTERS_STATE_IDLE)
	{
		ani = TELEPORTERS_ANI_IDLE;
	}

	animationSet->at(ani)->OldRender(x, y);
}

Teleporters::Teleporters(float x, float y, LPGAMEENTITY t)
{
	this->SetState(TELEPORTERS_STATE_IDLE);
	enemyType = TELEPORTERS;
	tag = TAG_TELEPORTERS;
	this->x = x;
	this->y = y;
	dam = 1;
	isAttack = 0;
	this->target = t;
	health = TELEPORTERS_MAXHEALTH;
	isActive = false;
	bbARGB = 250;
	isTransporting = true;
	isAttacking = false;
}

void Teleporters::AttackTarget(LPGAMEENTITY target) //tấn công nhân vật
{
	if (isTransporting)
	{
		this->SetState(TELEPORTERS_STATE_TRANSFORMING);
		isTransporting = false;
		isAttacking = true;
		//delayAttackTimer->Reset();
		delayAttackTimer->Start();
	}
	else if (!isTransporting && isAttacking && delayAttackTimer->IsTimeUp())
	{
		SetState(TELEPORTERS_STATE_ATTACKING);
		isAttacking = true;
		delayAttackTimer->Reset();
		delayAttackTimer->Start();
	}
}

void Teleporters::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
		case TELEPORTERS_STATE_DIE:
		{
			y += TELEPORTERS_BBOX_HEIGHT - TELEPORTERS_BBOX_HEIGHT_DIE + 1;
			break;
		}
		case TELEPORTERS_STATE_ATTACKING:
		{
			if (!isTransporting && isAttacking)
			{
				int x = 0 + rand() % (100 + 1 - 0);
				int y = 0 + rand() % (100 + 1 - 0);
				this->SetPosition(x, y);
				Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2 - 3.0f, y + TELEPORTERS_BBOX_HEIGHT/4, EntityType::TAG_TELEPORTERS, 0, -1);
				listBullet.push_back(bullet);
				isAttacking = false;
			}
			break;
		}
		case TELEPORTERS_STATE_TRANSFORMING:
		{
			break;
		}
		case TELEPORTERS_STATE_IDLE:
		{
			break;
		}
	}
}

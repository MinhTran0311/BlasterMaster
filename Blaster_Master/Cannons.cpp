#include "Cannons.h"

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
	if (health <= 0)
	{
		this->SetState(CANNONS_STATE_DIE);
		return;
	}
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
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->Getx(), target->Gety())) <= CANNONS_SITEATTACK_PLAYER)
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
	int ani = -1;
	if (state == CANNONS_STATE_DIE) {
		ani = CANNONS_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDoneDeath = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(nx, x, y - 3);
	}
	else {
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
}

Cannons::Cannons(float x, float y, LPGAMEENTITY t)
{
	this->SetState(CANNONS_STATE_IDLE);
	enemyType = CANNONS;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	this->dam = 1;
	isAttack = 0;
	this->target = t;
	health = CANNONS_MAXHEALTH;
	isActive = false;
	isAttackTop = false;
	changeAttackToTopBottom = false;
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
			//y += CANNONS_BBOX_HEIGHT - CANNONS_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			isActive = false;
			break;
		}
		case CANNONS_STATE_ATTACKING_TOP:
		{
			//Attack
			if (delayAttackTimer->IsTimeUp() && changeAttackToTopBottom)
			{
				LPBULLET bullet1 = new BigNavigatedEnemyBullet(x + CANNONS_BBOX_WIDTH / 2 - 3.0f, y + CANNONS_BBOX_HEIGHT, CANNONS, 0, 1, target);
				CGrid::GetInstance()->InsertGrid(bullet1);
				LPBULLET bullet2 = new BigNavigatedEnemyBullet(x + CANNONS_BBOX_WIDTH / 2 - 3.0f, y - 3.0f, CANNONS, 0, -1, target);
				CGrid::GetInstance()->InsertGrid(bullet2);
				delayAttackTimer->Reset();
				delayAttackTimer->Start();
				changeAttackToTopBottom = false;
			}
			break;
		}
		case CANNONS_STATE_ATTACKING_LEFT:
		{
			//Attack
			if (delayAttackTimer->IsTimeUp() && !changeAttackToTopBottom)
			{
				LPBULLET bullet1 = new BigNavigatedEnemyBullet(x + CANNONS_BBOX_WIDTH, y + CANNONS_BBOX_HEIGHT / 2 - 3.0f, CANNONS, 1, 0, target);
				CGrid::GetInstance()->InsertGrid(bullet1);
				LPBULLET bullet2 = new BigNavigatedEnemyBullet(x - 3.0f, y + CANNONS_BBOX_HEIGHT / 2 - 3.0f, CANNONS, -1, 0, target);
				CGrid::GetInstance()->InsertGrid(bullet2);
				delayAttackTimer->Reset();
				delayAttackTimer->Start();
				changeAttackToTopBottom = true;
			}
			break;
		}
		case CANNONS_STATE_IDLE:
		{
			break;
		}
	}
}

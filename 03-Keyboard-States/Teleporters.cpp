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
		this->SetState(TELEPORTERS_STATE_IDLE);
	}
#pragma endregion

}

void Teleporters::Render()
{
	RenderBoundingBox();

	int ani = -1;
	if (state == TELEPORTERS_STATE_DIE) {
		ani = TELEPORTERS_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDoneDeath = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(nx, x, y - 3);
	}
	else {
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
}

Teleporters::Teleporters(float x, float y, LPGAMEENTITY t)
{
	this->SetState(TELEPORTERS_STATE_IDLE);
	enemyType = TELEPORTERS;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	dam = 1;
	isAttack = 0;
	this->target = t;
	health = TELEPORTERS_MAXHEALTH;
	isActive = false;
	bbARGB = 0;
	isTransporting = true;
	isAttacking = false;
	isShooting = false;
	re_tranform = true;
	isIdling = false;
	enoughTimeIdle = false;
	teleTimesAttack = 0;
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
			if (teleTimesAttack == 7)
			{
				if (!isIdling)
				{
					SetState(TELEPORTERS_STATE_TRANSFORMING);
					isIdling = true;
				}
				else if (isIdling && !enoughTimeIdle)
				{
					SetState(TELEPORTERS_STATE_IDLE);
					enoughTimeIdle = true;
					delayIdleTimer->Start();
				}
				else if (isIdling && delayIdleTimer->IsTimeUp())
				{
					enoughTimeIdle = false;
					isIdling = false;
					teleTimesAttack = 0;
					isAttacking = false;
					isTransporting = true;
					delayIdleTimer->Reset();
					relaxAttackTimer->Start();
				}
			}
			else if (isShooting)
			{
				shootBulletTimer->Start();
				shootBulletToTarget();
				isShooting = false;
			}
			else if (!isTransporting && isAttacking && shootBulletTimer->IsTimeUp() && relaxAttackTimer->IsTimeUp())
			{
				int x = 0 + rand() % (100 + 1 - 0);
				int y = 0 + rand() % (100 + 1 - 0);
				this->SetPosition(x, y);
				shootBulletTimer->Reset();
				teleTimesAttack++;
				if (teleTimesAttack == 3 || teleTimesAttack == 6)
				{
					isShooting = true;
				}
				relaxAttackTimer->Reset();
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

void Teleporters::shootBulletToTarget()
{
	if (target->x == x)
	{
		if (target->y < y)
		{
			Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2, y + TELEPORTERS_BBOX_HEIGHT / 2, TELEPORTERS, 0, -1, target);
			CGrid::GetInstance()->InsertGrid(bullet);
		}
		else if (target->y > y)
		{
			Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2, y + TELEPORTERS_BBOX_HEIGHT / 2, TELEPORTERS, 0, 1, target);
			CGrid::GetInstance()->InsertGrid(bullet);
		}
	}
	else if (target->y == y)
	{
		if (target->x >= x)
		{
			Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2, y + TELEPORTERS_BBOX_HEIGHT / 2, TELEPORTERS, 1, 0, target);
			CGrid::GetInstance()->InsertGrid(bullet);
		}
		else if (target->x < x)
		{
			Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2, y + TELEPORTERS_BBOX_HEIGHT / 2, TELEPORTERS, -1, 0, target);
			CGrid::GetInstance()->InsertGrid(bullet);
		}
	}
	else if (target->x > x && target->y < y)
	{
		Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2, y + TELEPORTERS_BBOX_HEIGHT / 2, TELEPORTERS, 1, -1, target);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
	else if (target->x > x && target->y > y)
	{
		Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2, y + TELEPORTERS_BBOX_HEIGHT / 2, TELEPORTERS, 1, 1, target);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
	else if (target->x < x && target->y > y)
	{
		Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2, y + TELEPORTERS_BBOX_HEIGHT / 2, TELEPORTERS, -1, 1, target);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
	else if (target->x < x && target->y < y)
	{
		Bullet* bullet = new BigNavigatedEnemyBullet(x + TELEPORTERS_BBOX_WIDTH / 2, y + TELEPORTERS_BBOX_HEIGHT / 2, TELEPORTERS, -1, -1, target);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
}


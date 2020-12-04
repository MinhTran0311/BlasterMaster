#include "Eyeballs.h"

void Eyeballs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EYEBALLS_BBOX_WIDTH;
	bottom = y + EYEBALLS_BBOX_HEIGHT;
}

void Eyeballs::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	if (health <= 0)
	{
		this->SetState(EYEBALLS_STATE_DIE);
		return;
	}
#pragma region fall down
	//vy += EYEBALLS_GRAVITY * dt;
#pragma endregion
#pragma region Active

	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= EYEBALLS_SITEACTIVE_PLAYER)
	{
		FlyAndAttackTarget();
		isActive = true;
	}
	if (!isActive) SetState(EYEBALLS_STATE_IDLE);
		//else SetState(EYEBALLS_STATE_FLYING);
#pragma endregion
#pragma region Pre-collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;
	//SetState(EYEBALLS_STATE_FLYING);

	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			bricks.push_back(coObjects->at(i));

		// turn off collision when die 
		if (state != EYEBALLS_STATE_DIE)
			CalcPotentialCollisions(&bricks, coEvents);
	}
#pragma endregion
#pragma region coillision
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (!nx && !ny)
		{
			nx = -nx;
			vx = -vx;
			vy = -vy;
		}
		else if (!nx)
		{
			vy = -vy;
		}

		else if (!ny)
		{
			nx = -nx;
			vx = -vx;
		}

		//follow player
		//if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= EYEBALLS_SITEFOLLOW_PLAYER)
		//{
		//	//FlyAndAttackTarget();
		//}
		//else    //Wall or reaching the edges
		//{
		//	if (nx != 0)
		//	{
		//		this->nx = -this->nx;
		//	}
		//	if (ny != 0)
		//	{
		//		vy = 0;
		//		for (UINT i = 0; i < coEventsResult.size(); i++)
		//		{
		//			LPCOLLISIONEVENT e = coEventsResult.at(i);
		//			if (e->ny != 0)
		//			{
		//				RECT rect = static_cast<Brick*>(e->obj)->GetBBox();
		//				if (x + EYEBALLS_BBOX_WIDTH > rect.right)
		//				{
		//					this->nx = -this->nx;
		//					x += rect.right - (x + EYEBALLS_BBOX_WIDTH) - nx * 0.4f;
		//				}
		//				else if (x < rect.left)
		//				{
		//					this->nx = -this->nx;
		//					x += rect.left - x + nx * 0.4f;
		//				}
		//				break;
		//			}
		//		}
		//	}
		//}
	}
	//clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion

}

void Eyeballs::Render()
{
	RenderBoundingBox();
	if (vx > 0)
		nx = 1;
	else
		nx = -1;

	int ani = -1;
	if (state == EYEBALLS_STATE_DIE) {
		ani = EYEBALLS_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDoneDeath = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(nx, x, y - 2);
	}
	else
	{
		if (state == EYEBALLS_STATE_IDLE) {
			ani = EYEBALLS_ANI_IDLE;
		}
		else if (state == EYEBALLS_STATE_FLYING || state == EYEBALLS_STATE_ATTACKING) {
			ani = EYEBALLS_ANI_FLYING;
		}
		animationSet->at(ani)->Render(nx, x, y);
		//RenderBoundingBox();
	}
}

Eyeballs::Eyeballs(float x, float y, LPGAMEENTITY t)
{
	SetState(EYEBALLS_STATE_IDLE);
	enemyType = EYEBALLS;
	tag = ENEMY;
	this->x = x;
	this->y = y;
	dam = 1;
	nx = 1;
	this->target = t;
	health = EYEBALLS_MAXHEALTH;
	isActive = false;
	bbARGB = 0;
	canAttack = false;
	canFly = false;
	canIdle = true;
}

void Eyeballs::FlyAndAttackTarget()
{
	/*if (state == EYEBALLS_STATE_IDLE && canFly)
	{
		canFly = false;
		SetState(EYEBALLS_STATE_IDLE);
		idleTimer->Start();
		if (idleTimer->IsTimeUp())
		{
			DebugOut(L"########### %d\n", 80000);
			SetState(EYEBALLS_STATE_FLYING);
			canAttack = true;
			flyOrAttackTimer->Start();
			idleTimer->Reset();
		}
	}
	else if (canAttack && flyOrAttackTimer->IsTimeUp())
	{
		DebugOut(L"########### %d\n", 90000);
		SetState(EYEBALLS_STATE_IDLE);
		idleTimer->Start();
		if (idleTimer->IsTimeUp())
		{
			SetState(EYEBALLS_STATE_ATTACKING);
			flyOrAttackTimer->Reset();
			flyOrAttackTimer->Start();
			canAttack = false;
			canFly = true;
			idleTimer->Reset();
		}
	}
	else if (canFly && flyOrAttackTimer->IsTimeUp())
	{
		SetState(EYEBALLS_STATE_IDLE);
		idleTimer->Start();
		if (idleTimer->IsTimeUp())
		{
			SetState(EYEBALLS_STATE_FLYING);
			flyOrAttackTimer->Reset();
			flyOrAttackTimer->Start();
			canFly = false;
			canAttack = true;
			idleTimer->Reset();
		}
	}*/
	//if (canIdle)
	//{
	//	SetState(EYEBALLS_STATE_IDLE);
	//	canIdle = false;
	//	flyOrAttackTimer->Start();
	//}
	if (state == EYEBALLS_STATE_IDLE)
	{
		SetState(EYEBALLS_STATE_FLYING);
		canAttack = true;
		canFly = false;
		//canIdle = true;
		flyOrAttackTimer->Start();
	}
	else if (canFly && flyOrAttackTimer->IsTimeUp())
	{
		SetState(EYEBALLS_STATE_FLYING);
		flyOrAttackTimer->Reset();
		flyOrAttackTimer->Start();
		canFly = false;
		canAttack = true;
	}
	else if (canAttack && flyOrAttackTimer->IsTimeUp())
	{
		SetState(EYEBALLS_STATE_ATTACKING);
		flyOrAttackTimer->Reset();
		flyOrAttackTimer->Start();
		canAttack = false;
		canFly = true;
	}
}

void Eyeballs::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
		case EYEBALLS_STATE_DIE:
			isActive = false;
			//y += EYEBALLS_BBOX_HEIGHT - EYEBALLS_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case EYEBALLS_STATE_FLYING:
		{
			setRandomVxVy(vx, vy);
			break;
		}
		case EYEBALLS_STATE_ATTACKING:
		{
			if (target->x == x)
			{
				if (target->y < y)
				{
					Bullet* bullet = new BigNavigatedEnemyBullet(x + EYEBALLS_BBOX_WIDTH / 2, y + EYEBALLS_BBOX_HEIGHT / 2, EYEBALLS, 0, -1, target);
					CGrid::GetInstance()->InsertGrid(bullet);
				}
				else if (target->y > y)
				{
					Bullet* bullet = new BigNavigatedEnemyBullet(x + EYEBALLS_BBOX_WIDTH / 2, y + EYEBALLS_BBOX_HEIGHT / 2, EYEBALLS, 0, 1, target);
					CGrid::GetInstance()->InsertGrid(bullet);
				}
			}
			else if (target->y == y)
			{
				if (target->x >= x)
				{
					Bullet* bullet = new BigNavigatedEnemyBullet(x + EYEBALLS_BBOX_WIDTH / 2, y + EYEBALLS_BBOX_HEIGHT / 2, EYEBALLS, 1, 0, target);
					CGrid::GetInstance()->InsertGrid(bullet);
				}
				else if (target->x < x)
				{
					Bullet* bullet = new BigNavigatedEnemyBullet(x + EYEBALLS_BBOX_WIDTH / 2, y + EYEBALLS_BBOX_HEIGHT / 2, EYEBALLS, -1, 0, target);
					CGrid::GetInstance()->InsertGrid(bullet);
				}
			}
			else if (target->x > x && target->y < y)
			{
				Bullet* bullet = new BigNavigatedEnemyBullet(x + EYEBALLS_BBOX_WIDTH / 2, y + EYEBALLS_BBOX_HEIGHT / 2, EYEBALLS, 1, -1, target);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			else if (target->x > x && target->y > y)
			{
				Bullet* bullet = new BigNavigatedEnemyBullet(x + EYEBALLS_BBOX_WIDTH / 2, y + EYEBALLS_BBOX_HEIGHT / 2, EYEBALLS, 1, 1, target);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			else if (target->x < x && target->y > y)
			{
				Bullet* bullet = new BigNavigatedEnemyBullet(x + EYEBALLS_BBOX_WIDTH / 2, y + EYEBALLS_BBOX_HEIGHT / 2, EYEBALLS, -1, 1, target);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			else if (target->x < x && target->y < y)
			{
				Bullet* bullet = new BigNavigatedEnemyBullet(x + EYEBALLS_BBOX_WIDTH / 2, y + EYEBALLS_BBOX_HEIGHT / 2, EYEBALLS, -1, -1, target);
				CGrid::GetInstance()->InsertGrid(bullet);
			}
			vx = 0;
			vy = 0;
			break;
		}
		case EYEBALLS_STATE_IDLE:
		{
			vx = 0;
			vy = 0;
			break;
		}
	}
}

void Eyeballs::setRandomVxVy(float& vx, float& vy)
{
	vx = r->getRandomFloat(0.0001f, EYEBALLS_FLYING_SPEED);
	vy = sqrt(2 * EYEBALLS_FLYING_SPEED * EYEBALLS_FLYING_SPEED - vx * vx);
}

#include "Floaters.h"

void Floaters::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BBOX_WIDTH;
	bottom = y + BBOX_HEIGHT;
}

void Floaters::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	if (health <= 0)
	{
		this->SetState(FLOATERS_STATE_DIE);
		return;
	}
#pragma region fall down
	//vy += WORM_GRAVITY * dt;
#pragma endregion
	Attack();
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
		if (state != FLOATERS_STATE_DIE)
			CalcPotentialCollisions(&bricks, coEvents);
	}
#pragma endregion

#pragma region Timer
	//if (canAttack)
	//{
	//	if (cooldownTimer->IsTimeUp())
	//	{
	//		cooldownTimer->GetStartTime
	//			isImmortaling = false;
	//		immortalTimer->Reset();
	//	}

	//}
	
#pragma endregion
#pragma region coillision
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

		x += min_tx * dx + nx * 0.4f; // ko dính vào tường
		y += min_ty * dy + ny * 0.4f;// ko dính vào tường

		/*setRandomVxVy(vx, vy);*/
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
			



	}
	//clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
#pragma region Active
	if (!isActive) {
		vx = 0;
		vy = 0;
	}
	else SetState(FLOATERS_STATE_FLY);
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= ACTIVE_RANGE)
	{
		if (!firstTimeActive)
		{
			setRandomVxVy(vx, vy);
			isActive = true;
			firstTimeActive = true;
			Attack();
		}
	}
#pragma endregion

}

void Floaters::Render()
{
	//RenderBoundingBox();
	int ani;
	if (state == FLOATERS_STATE_DIE)
	{
		ani = FLOATERS_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDoneDeath = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(nx, x, y - 2);
	}
	else if (state == FLOATERS_STATE_FLY)
	//else if (cooldownTimer->IsTimeUp())
	{
		ani = FLOATERS_ANI_FLY;
		animationSet->at(ani)->Render(nx,x, y);
	}
}

Floaters::Floaters(float x, float y, LPGAMEENTITY t)
{
	this->target = t;
	SetState(FLOATERS_STATE_FLY);
	enemyType = FLOATERS;
	tag = ENEMY;
	this->x = x;
	this->y = y;
	nx = -1;
	isTargeting = 0;
	health = MAXHEALTH;
	isActive = false;
	bbARGB = 250;
	dam = 1;
	firstTimeActive = false;
	isDoneDeath = false;
}

void Floaters::Attack(LPGAMEENTITY target) //đi theo nhân vật
{
	if (canAttack)
	{
		canAttack = false;
		cooldownTimer->Start();
	}
	else if (cooldownTimer->IsTimeUp())
	{
		cooldownTimer->Reset();
		canAttack = true;
		shootBulletToTarget();
	}
}

void Floaters::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
		case FLOATERS_STATE_DIE:
		{
			//y += BBOX_HEIGHT/2 + 1;
			vx = 0;
			vy = 0;
			isActive = false;
			break;
		}
		case FLOATERS_STATE_FLY:
		{
			setRandomVxVy(vx, vy);
			break;
		}
	}
}

bool Floaters::inTargetRange()
{
	return GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= TARGET_RANGE;
}

void Floaters::setRandomVxVy(float& vx, float& vy)
{
	//vx = r->getRandomFloat(0.0001f, MOVING_SPEED);
	//vy = sqrt(2 * MOVING_SPEED * MOVING_SPEED - vx * vx);
	if (vx > 0)
	{
		vx = r->getRandomFloat(0.001f, MOVING_SPEED);
	}
	else vx = -r->getRandomFloat(0.001f, MOVING_SPEED);
	if (vy > 0)
		vy = sqrt(2 * MOVING_SPEED * MOVING_SPEED - vx * vx);
	else vy = -sqrt(2 * MOVING_SPEED * MOVING_SPEED - vx * vx);
}

void Floaters::shootBulletToTarget()
{
	if (target->Getx() >= x && target->Gety() <= y)
	{
		LPBULLET bullet = new SmallNavigatedEnemyBullet(x + BBOX_WIDTH / 2, y + BBOX_HEIGHT / 2, FLOATERS, 1, -1, target);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
	else if (target->Getx() >= x && target->Gety() > y)
	{
		LPBULLET bullet = new SmallNavigatedEnemyBullet(x + BBOX_WIDTH / 2, y + BBOX_HEIGHT / 2, FLOATERS, 1, 1, target);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
	else if (target->Getx() <= x && target->Gety() >= y)
	{
		LPBULLET bullet = new SmallNavigatedEnemyBullet(x + BBOX_WIDTH / 2, y + BBOX_HEIGHT / 2, FLOATERS, -1, 1, target);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
	else if (target->Getx() <= x && target->Gety() <= y)
	{
		LPBULLET bullet = new SmallNavigatedEnemyBullet(x + BBOX_WIDTH / 2, y + BBOX_HEIGHT / 2, FLOATERS, -1, -1, target);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
}
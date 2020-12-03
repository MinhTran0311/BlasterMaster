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
		}
		
	}
#pragma endregion

}

void Floaters::Render()
{
	//RenderBoundingBox();
	int ani;
	if (health <= 0)
	{
		ani = FLOATERS_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDoneDeath = true;
		}
		animationSet->at(ani)->Render(nx,x, y);
	}
	else if (true)
	//else if (cooldownTimer->IsTimeUp())
	{
		ani = FLOATERS_ANI_FLY;
		animationSet->at(ani)->Render(nx,x, y);
		//animationSet->at(ani)->OldRender(x, y);

	}
	/*else 
		if (!delayTimeranishot->IsTimeUp())
	{
		ani = FLOATERS_ANI_ATTACK;
		animationSet->at(ani)->OldRender(x, y);

	}*/


	/*for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Render();
	}*/
	//RenderBoundingBox();
}

Floaters::Floaters(float x, float y, LPGAMEENTITY t)
{
	SetState(FLOATERS_STATE_FLY);
	enemyType = FLOATERS;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	nx = -1;
	isTargeting = 0;
	this->target = t;
	health = MAXHEALTH;
	isActive = false;
	bbARGB = 250;
}

void Floaters::Attack(LPGAMEENTITY target) //đi theo nhân vật
{
	//if ((target->x - this->x) > 0)
	//{
	//	this->nx = 1;
	//	vx = WORM_WALKING_SPEED;
	//}
	//else
	//{
	//	vx = -WORM_WALKING_SPEED;
	//	this->nx = -1;
	//}
	if (canAttack)
	{
		if (target != NULL)
		{

		}
		else
		{

		}
	}
	

}

void Floaters::SetState(int state)
{
	switch (state)
	{
	case FLOATERS_STATE_DIE:
		//y += BBOX_HEIGHT/2 + 1;
		vx = 0;
		vy = 0;
		isActive = false;
		break;

	case FLOATERS_STATE_FLY:
		setRandomVxVy(vx, vy);
		break;

	//case FLOATERS_STATE_ATTACK:
	//	cooldownTimer->Reset(randomTimeAttack());
	//	delayTimer->Start();
	//	break;
	
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


#include "Insects.h"

void Insects::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BBOX_WIDTH;
	bottom = y + BBOX_HEIGHT;
}

void Insects::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	if (health <= 0)
	{
		this->SetState(INSECTS_STATE_DIE);
		return;
	}
#pragma region fly
	//vy += WORM_GRAVITY * dt;
	fly(dt);
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
		if (state != INSECTS_STATE_DIE)
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

		//setRandomVxVy(vx, vy);
		if (!nx && !ny)
		{
			nx = -nx;
			vx = -vx;
			//vy = -vy;
		}
		else if (!nx)
		{
			//vy = -vy;
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
	if (!isActive)
	{
		vx = 0;
		vy = 0;
	}
	else SetState(INSECTS_STATE_FLY);
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

void Insects::Render()
{
	//RenderBoundingBox();
	if (vx > 0)
	{
		nx = 1;
	}
	else nx = -1;

	int ani;
	if (health <= 0)
	{
		ani = INSECTS_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDoneDeath = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(nx,x, y);
	}
	else if (true)
		//else if (cooldownTimer->IsTimeUp())
	{
		ani = INSECTS_ANI_FLY;
		animationSet->at(ani)->Render(nx, x, y);
		//animationSet->at(ani)->OldRender(x, y);

	}
	/*else
		if (!delayTimeranishot->IsTimeUp())
	{
		ani = INSECTS_ANI_ATTACK;
		animationSet->at(ani)->OldRender(x, y);

	}*/

	//RenderBoundingBox();
}

Insects::Insects(float x, float y, LPGAMEENTITY t)
{
	SetState(INSECTS_STATE_FLY);
	enemyType = INSECT;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	nx = -1;
	ny = -1;
	isTargeting = 0;
	this->target = t;
	health = MAXHEALTH;
	isActive = false;
	setRandomFlyDropRange();
	bbARGB = 250;
	originalY = this->y;
	maxdrop = flyDropRange + originalY;
}



void Insects::SetState(int state)
{
	switch (state)
	{
	case INSECTS_STATE_DIE:
		//y += BBOX_HEIGHT / 2 + 1;
		isActive = false;
		vx = 0;
		vy = 0;
		break;

	case INSECTS_STATE_FLY:

		setRandomVx();


		break;

		//case INSECTS_STATE_ATTACK:
		//	cooldownTimer->Reset(randomTimeAttack());
		//	delayTimer->Start();
		//	break;

	}

}

bool Insects::inTargetRange()
{
	return GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= TARGET_RANGE;
}

void Insects::setRandomVxVy(float& vx, float& vy)
{
	//vx = r->getRandomFloat(0.01f, MOVING_SPEED);
	//vy = sqrt(2 * MOVING_SPEED * MOVING_SPEED - vx * vx);
	if (vx > 0)
	{
		vx = r->getRandomFloat(0.001f, MOVING_SPEED);
	}
	else vx = -r->getRandomFloat(0.001f, MOVING_SPEED);
	if (vy>0)
		vy = sqrt(2 * MOVING_SPEED * MOVING_SPEED - vx * vx);
	else vy = -sqrt(2 * MOVING_SPEED * MOVING_SPEED - vx * vx);
	//int nx = r->getRandomInt(1, 3);
	//int ny = r->getRandomInt(1,3);
	//ny++;
	//if (nx%2==0)
	//{
	//	vx = -vx;
	//}
	////if (ny % 2 ==0)
	////{
	////	vy = -vy;
	////}


}

void Insects::setRandomVx()
{
	vx = r->getRandomFloat(0.001f, MOVING_SPEED);
}

void Insects::fly(const DWORD &dt)
{
	

	if (isDroping&&y<maxdrop)
	{
		vy = 0.01f;
	}
	else 
	{
		if (!isDroping&&y > originalY)
		{
			vy = -0.07f;
		}
		else isDroping = !isDroping;
	}
}

void Insects::setRandomFlyDropRange()
{
	this->flyDropRange = r->getRandomInt(MIN_FLY_DROP_RANGE, MAX_FLY_DROP_RANGE);
}
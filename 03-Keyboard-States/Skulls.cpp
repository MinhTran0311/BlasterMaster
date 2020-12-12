#include "Skulls.h"

void Skulls::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BBOX_WIDTH;
	bottom = y + BBOX_HEIGHT;
}

void Skulls::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);

#pragma region fall down
	//vy += WORM_GRAVITY * dt;
#pragma endregion
#pragma region Pre-collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;
	if (this->health <= 0) { SetState(SKULLS_STATE_DIE); return; }
	if (this->state == SKULLS_STATE_ATTACK && time == 20) SetState(SKULLS_STATE_STOP);
	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			bricks.push_back(coObjects->at(i));

		// turn off collision when die 
		if (state != SKULLS_STATE_DIE)
			CalcPotentialCollisions(&bricks, coEvents);
	}
	
#pragma endregion



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

	//if (!isActive) vx = 0;
	//else SetState(SKULLS_STATE_FLY);
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= SKULLS_SITEACTIVE_PLAYER)
	{
		isActive = true;
		Attack(target);
		/*if (this->x == this->target->x)
		{
			Attack();
		}*/
	}
#pragma endregion

}

void Skulls::Render()
{
	//RenderBoundingBox();
	int ani;
	if (this->state == SKULLS_STATE_DIE)
	{
		ani = SKULLS_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDoneDeath = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(nx, x, y-2);
	}
	else if (this->state == SKULLS_STATE_FLY)
		//else if (cooldownTimer->IsTimeUp())
	{
		if (vx > 0)
			nx = 1;
		else
			nx = -1;
		ani = SKULLS_ANI_FLY;
		animationSet->at(ani)->Render(nx, x, y);


	}
	else if (this->state == SKULLS_STATE_ATTACK)
	{
		
		ani = SKULLS_ANI_ATTACK;
		animationSet->at(ani)->Render(nx, x, y);

	}
	else if (this->state ==SKULLS_STATE_STOP)
	{

		ani = SKULLS_ANI_FLY;
		animationSet->at(ani)->Render(nx, x, y);

	}
	
}

Skulls::Skulls(float x, float y, LPGAMEENTITY t)
{
	SetState(SKULLS_STATE_FLY);
	enemyType = SKULLS;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	nx = 1;
	isTargeting = 0;
	this->target = t;
	health = MAXHEALTH;
	isActive = false;
	bbARGB = 250;
}

void Skulls::Attack(LPGAMEENTITY target) //tấn công tại vị trí nhân vật
{
	if (abs(target->x -this->x) < 50 && time < 100) {
		/* this->nx= target->nx;*/
		SetState(SKULLS_STATE_ATTACK);
		if (!Attacked) {
			Bullet* bullet = new SkullBullet(this->x, this->y, this->nx, target);
			CGrid::GetInstance()->InsertGrid(bullet);
			Attacked = true;
		}
		
	}
}

void Skulls::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case SKULLS_STATE_DIE:
		//y += BBOX_HEIGHT / 2 + 1;
		vx = 0;
		vy = 0;
		isActive = false;
		break;

	case SKULLS_STATE_FLY:
		if (nx > 0)
		{
			vx = MOVING_SPEED;
		}
		else
		{
			vx = -MOVING_SPEED;
		}
		break;

	case SKULLS_STATE_ATTACK:
			
		vx = 0;
		vy = -MOVING_SPEED;
		if(time <30)
		time++;
		break;

	
	case SKULLS_STATE_STOP:
		time = 120;
		vx = 0;
		vy = 0;
		break;

	}

}

bool Skulls::inTargetRange()
{
	return GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= TARGET_RANGE;
}

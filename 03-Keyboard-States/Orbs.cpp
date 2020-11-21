#include "Orbs.h"
void Orbs::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BBOX_WIDTH;
	bottom = y + BBOX_HEIGHT;
}

void Orbs::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	
	
#pragma region fall down
	//vy += WORM_GRAVITY * dt;
#pragma endregion
#pragma region Pre-collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;
	if (isflip == true) { SetState(ORBS_STATE_FLIP); }
	if (time == 19) { SetState(ORBS_STATE_FLY); isflip = false; }
	if (collectionX == x)time = 0;
	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			bricks.push_back(coObjects->at(i));

		// turn off collision when die 
		if (state != ORBS_STATE_DIE)
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
			isflip = true;
			this->collectionX = this->x;
			nx = -nx;
			vx = -vx;
		}
		




	}
	//clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
#pragma region Active
	/*if (!isActive) vx = 0;
	else SetState(ORBS_STATE_FLY);
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= ORBS_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}*/
#pragma endregion

}

void Orbs::Render()
{
	//RenderBoundingBox();
	if (vx > 0)
		nx = 1;
	else
		nx = -1;


	int ani;
	if (health <= 0)
	{
		ani = ORBS_ANI_DIE;
		animationSet->at(ani)->OldRender(x, y);
	}
	else if (!isflip )
		//else if (cooldownTimer->IsTimeUp())
	{
		ani = ORBS_ANI_FLY;
		animationSet->at(ani)->Render(nx, x, y);
		//animationSet->at(ani)->OldRender(x, y);

	}
	else if (isflip && time <20)
	{

		ani = ORBS_ANI_FLIP;
		animationSet->at(ani)->Render(nx, x, y);
		
		
		
		
	}


	/*for (int i = 0; i < bullet.size(); i++)
	{
		bullet.at(i)->Render();
	}*/
	//RenderBoundingBox();
}

Orbs::Orbs(float x, float y, LPGAMEENTITY t)
{

	SetState(ORBS_STATE_FLY);
	enemyType = ORBS;
	tag = Tag_Orbs;
	this->x = x;
	this->y = y;
	nx = -1;
	//dam = 1;
	isTargeting = 0;
	this->target = t;
	health = MAXHEALTH;
	isActive = false;
	bbARGB = 250;
}

void Orbs::Attack(LPGAMEENTITY target) //đi theo nhân vật
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

void Orbs::SetState(int state)
{
	switch (state)
	{
	case ORBS_STATE_DIE:
		y += BBOX_HEIGHT / 2 + 1;
		vx = 0;
		vy = 0;
		break;
	
	case ORBS_STATE_FLIP:
		vx = 0;
		time++;
		break;
	
	case ORBS_STATE_ATTACK:
		

		break;

	case ORBS_STATE_FLY:
		
		if (time == 21)time = 0;
		if (nx > 0)
		{
			vx = MOVING_SPEED;
		}
		else
		{
			vx = -MOVING_SPEED;
		}
		break;

	
	}

}

bool Orbs::inTargetRange()
{
	return GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= TARGET_RANGE;
}

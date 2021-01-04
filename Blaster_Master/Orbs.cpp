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
	if (health <= 0)
	{
		SetState(ORBS_STATE_DIE);
		return;
	}
	if (isfly) { SetState(ORBS_STATE_FLY);  }
	else {
		SetState(ORBS_STATE_IDLE);
	}
	if (this->pointX != this->x || this->pointY != this->y) { canflip=true; }
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

#pragma region coillision
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny=0;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//x += min_tx * dx + nx * 3.6f; // ko dính vào tường
		//y += min_ty * dy + ny * 3.6f;// ko dính vào tường

		//setRandomVxVy(vx, vy);
		
		
		if (!ny)
		{
			
			this->vx = -this->vx;
			
		}
		if (!nx)
		{
			this->vy = -this->vy;
			
		}
		
		if (!this->vx || !this->vy) {
			if (canflip) {
				isfly = false;
				SetState(ORBS_STATE_FLIP);
				this->pointX = this->x;
				this->pointY = this->y;
			}
			
		}
		
	}
	//clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
#pragma region Active
	if (!isActive) return;
	/*else SetState(ORBS_STATE_FLY);*/
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->Getx(), target->Gety())) <= TARGET_RANGE)
	{
		isfly = true;
		if (mode == 1) {
			Attack(target);
		}
	}
	else isfly = false;

#pragma endregion

}

void Orbs::Render()
{
	if (vx >= 0)
		nx = 1;
	else
		nx = -1;
	if (vy > 0)
		ny = 1;
	else
		ny = -1;


	int ani;
	if (this->state == ORBS_STATE_DIE)
	{
		ani = ORBS_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == animationSet->at(ani)->GetLastFrameIndex())
		{
			isDoneDeathAni = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(1,x, y);
	}
	else if (this->state == ORBS_STATE_FLY || this->state == ORBS_STATE_IDLE)
		//else if (cooldownTimer->IsTimeUp())
	{
		ani = ORBS_ANI_FLY;
		animationSet->at(ani)->Render(nx, x, y);

	}
	else if (this->state == ORBS_STATE_FLIP)
	{

		ani = ORBS_ANI_FLIP;
		animationSet->at(ani)->Render(nx, x, y);
		if (animationSet->at(ani)->GetFrame() == 2) {
			isfly = true;
			canflip = false;
		}

	}
}

Orbs::Orbs(float x, float y, LPGAMEENTITY t, int orb_mode)
{
	this->mode = orb_mode;
	SetState(ORBS_STATE_IDLE);
	enemyType = ORBS;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	nx = 1;
	ny = -1;
	this->dam = 1;
	isTargeting = 0;
	this->target = t;
	health = MAXHEALTH;
	isActive = true;
	canflip = true;
	isfly = true;
	isAttack = false;
}

void Orbs::Attack(LPGAMEENTITY target) //đi theo nhân vật
{
	
	if (abs(target->Getx() - this->x) < 20 && canflip && abs(target->Gety() - this->y) < 60) {
		isAttack == true;
		/*if (this->r->getRandomInt(1, 2) == 1) { this->vy = FLYING_SPEED; }
		if (this->r->getRandomInt(1, 2) == 2) { this->vy = -FLYING_SPEED; }*/
		//else this->vy = -FLYING_SPEED;
		this->vy = FLYING_SPEED;
		if (target->Getx() - this->x < 0)nx = -1;
		else nx = 1;
		SetState(ORBS_STATE_FLIP);
	}
	else if (abs(target->Gety() - this->y) < 20) { 
		this->vy = -FLYING_SPEED; 
		SetState(ORBS_STATE_FLIP);
	}
	else this->vy = 0;
	
}

void Orbs::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case ORBS_STATE_DIE:
		isActive = false;
		/*y += BBOX_HEIGHT / 2 + 1;*/
		vx = 0;
		vy = 0;
		break;
	
	case ORBS_STATE_FLIP:
		if (isAttack) {
			vx = 0;
			vy = 0;
		}
		break;
	
	case ORBS_STATE_ATTACK:
		break;

	case ORBS_STATE_FLY:
	{
		isAttack = false;
		vy = 0;
		if (nx > 0)
			vx = MOVING_SPEED;
		else
			vx = -MOVING_SPEED;

		break;
	}
	case ORBS_STATE_IDLE:
	{
		vx = 0;
		vy = 0;
	}
	default: 
		break;
	}

}

bool Orbs::inTargetRange()
{
	return GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->Getx(), target->Gety())) <= TARGET_RANGE;
}

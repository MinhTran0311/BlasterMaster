#include "Jumpers.h"
void Jumpers::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + JUMPER_BBOX_WIDTH;
	bottom = y + JUMPER_BBOX_HEIGHT;
}

void Jumpers::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	//if (!isActive)return;
#pragma region fall down
	vy += JUMPER_GRAVITY * dt;
#pragma endregion
#pragma region Pre-collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;
	if (this->health <= 0) { SetState(JUMPER_STATE_DIE); return; }
	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			bricks.push_back(coObjects->at(i));

		// turn off collision when die 
		if (state != JUMPER_STATE_DIE)
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
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//follow player
		if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= JUMPER_SITEFOLLOW_PLAYER)
		{
			FollowTarget(target);
		}
		else    //Wall or reaching the edges
		{
			if (nx != 0)
			{
				this->nx = -this->nx;
			}
			if (ny != 0)
			{
				vy = 0;
				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult.at(i);
					if (e->ny != 0)
					{
						RECT rect = static_cast<Brick*>(e->obj)->GetBBox();
						if (x + JUMPER_BBOX_WIDTH > rect.right)
						{
							this->nx = -this->nx;
							x += rect.right - (x + JUMPER_BBOX_WIDTH) - nx * 0.4f;
						}
						else if (x < rect.left)
						{
							this->nx = -this->nx;
							x += rect.left - x + nx * 0.4f;
						}
						break;
					}
				}
			}
		}
	}
	//clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
#pragma region Active
	if (!isActive) return;
	else SetState(JUMPER_STATE_WALKING);
	
#pragma endregion

}

void Jumpers::Render()
{
	if (vx > 0)
		nx = 1;
	else
		nx = -1;

	int ani = JUMPER_ANI_WALKING;
	if (state == JUMPER_STATE_DIE) {
		ani = JUMPER_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isDoneDeath = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(nx, x, y - 2);
	}
	else
		animationSet->at(ani)->Render(nx, x, y);
}

Jumpers::Jumpers(float x, float y, LPGAMEENTITY t)
{
	SetState(JUMPER_STATE_WALKING);
	enemyType = JUMPERS;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	this->dam = 1;
	nx = -1;
	isFollow = 0;
	this->target = t;
	health = JUMPER_MAXHEALTH;
	isActive = true;
}

void Jumpers::FollowTarget(LPGAMEENTITY target) //đi theo nhân vật
{
	
	if (indexFollow < 1) {
		SetState(JUMPER_STATE_JUMP);
		indexFollow++;
	}
	else indexFollow++;
	if (indexFollow == 50)indexFollow = 0;
	
	if ((target->x - this->x) > 0)
	{
		this->nx = 1;
		vx = JUMPER_WALKING_SPEED;
		
	}
	else
	{
		vx = -JUMPER_WALKING_SPEED;
		this->nx = -1;
		
	}
	

}

void Jumpers::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case JUMPER_STATE_DIE:
		//y += JUMPER_BBOX_HEIGHT - JUMPER_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isActive = false;
		break;

	case JUMPER_STATE_JUMP:
		vy = -JUMPER_JUMP_SPEED_Y;
		break;
	case JUMPER_STATE_WALKING:

		if (nx > 0)
		{
			vx = JUMPER_WALKING_SPEED;
		}
		else
		{
			vx = -JUMPER_WALKING_SPEED;
		}
		break;
	}
}

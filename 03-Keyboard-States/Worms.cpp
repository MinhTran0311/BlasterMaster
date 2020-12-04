#include "Worms.h"

void Worm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WORM_BBOX_WIDTH;
	bottom = y + WORM_BBOX_HEIGHT;
}

void Worm::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	if (health <= 0)
	{
		this->SetState(WORM_STATE_DIE);
		return;
	}
#pragma region fall down
	vy += WORM_GRAVITY * dt;
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
		if (state != WORM_STATE_DIE)
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
		if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= WORM_SITEFOLLOW_PLAYER)
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
						if (x + WORM_BBOX_WIDTH > rect.right)
						{
							this->nx = -this->nx;
							x += rect.right - (x + WORM_BBOX_WIDTH) - nx * 0.4f;
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
	if (!isActive) vx = 0;
	else SetState(WORM_STATE_WALKING);
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= WORM_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
#pragma endregion

}

void Worm::Render()
{
	//RenderBoundingBox();
	//if (vx > 0)
	//	nx = 1;
	//else
	//	nx = -1;

	int ani = WORM_ANI_WALKING;
	if (state == WORM_STATE_DIE) {
		ani = WORM_ANI_DIE;
		//DebugOut(L"So sprite %d\n", animationSet->at(ani)->GetFrame());
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			animationSet->at(ani)->ResetCurrentFrame();
			isDoneDeath = true;
		}
		animationSet->at(ani)->Render(nx, x, y  + WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE-3);
	}
	else
		animationSet->at(ani)->Render(nx, x, y);
	//RenderBoundingBox();
}

Worm::Worm(float x, float y, LPGAMEENTITY t)
{
	SetState(WORM_STATE_WALKING);
	enemyType = WORM;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	nx = -1;
	isFollow = 0;
	this->target = t;
	health = WORM_MAXHEALTH;
	isActive = false;
	bbARGB = 250;
}

void Worm::FollowTarget(LPGAMEENTITY target) //đi theo nhân vật
{
	if ((target->x - this->x) > 0)
	{
		this->nx = 1;
		vx = WORM_WALKING_SPEED;
	}
	else
	{
		vx = -WORM_WALKING_SPEED;
		this->nx = -1;
	}

}

void Worm::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case WORM_STATE_DIE:
	{
		//y += WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		isActive = false;
		break;
	}
	case WORM_STATE_WALKING:
		if (nx > 0)
		{
			vx = WORM_WALKING_SPEED;
		}
		else
		{
			vx = -WORM_WALKING_SPEED;
		}
	}
}

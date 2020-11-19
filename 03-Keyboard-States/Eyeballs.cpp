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

#pragma region fall down
	vy += EYEBALLS_GRAVITY * dt;
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
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		//follow player
		if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= EYEBALLS_SITEFOLLOW_PLAYER)
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
						if (x + EYEBALLS_BBOX_WIDTH > rect.right)
						{
							this->nx = -this->nx;
							x += rect.right - (x + EYEBALLS_BBOX_WIDTH) - nx * 0.4f;
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
	else SetState(EYEBALLS_STATE_FLYING);
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= EYEBALLS_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
#pragma endregion
}

void Eyeballs::Render()
{
	RenderBoundingBox();
	if (vx > 0)
		nx = 1;
	else
		nx = -1;

	int ani = EYEBALLS_ANI_FLYING;
	if (state == EYEBALLS_STATE_DIE) {
		ani = EYEBALLS_ANI_DIE;
	}

	animationSet->at(ani)->Render(nx, x, y);
	//RenderBoundingBox();
}

Eyeballs::Eyeballs(float x, float y, LPGAMEENTITY t)
{
	SetState(EYEBALLS_STATE_FLYING);
	enemyType = EYEBALLS;
	tag = Tag_Eyeballs;
	this->x = x;
	this->y = y;
	dam = 1;
	nx = -1;
	isFollow = 0;
	this->target = t;
	health = EYEBALLS_MAXHEALTH;
	isActive = false;
	bbARGB = 250;
}

void Eyeballs::FollowTarget(LPGAMEENTITY target)
{
	if ((target->x - this->x) > 0)
	{
		this->nx = 1;
		vx = EYEBALLS_WALKING_SPEED;
	}
	else
	{
		vx = -EYEBALLS_WALKING_SPEED;
		this->nx = -1;
	}
}

void Eyeballs::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case EYEBALLS_STATE_DIE:
		y += EYEBALLS_BBOX_HEIGHT - EYEBALLS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case EYEBALLS_STATE_FLYING:
		if (nx > 0)
		{
			vx = EYEBALLS_WALKING_SPEED;
		}
		else
		{
			vx = -EYEBALLS_WALKING_SPEED;
		}
	}
}

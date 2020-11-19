#include "Teleporters.h"

void Teleporters::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TELEPORTERS_BBOX_WIDTH;
	bottom = y + TELEPORTERS_BBOX_HEIGHT;
}

void Teleporters::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);

#pragma region fall down
	vy += TELEPORTERS_GRAVITY * dt;
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
		if (state != TELEPORTERS_STATE_DIE)
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
		if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= TELEPORTERS_SITEATTACK_PLAYER)
		{
			AttackTarget(target);
		}
		else    //Wall or reaching the edges
		{
			finish_transformation = false;
			//state = TELEPORTERS_STATE_IDLE;
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
						if (x + TELEPORTERS_BBOX_WIDTH > rect.right)
						{
							this->nx = -this->nx;
							x += rect.right - (x + TELEPORTERS_BBOX_WIDTH) - nx * 0.4f;
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
	else SetState(TELEPORTERS_STATE_ATTACKING);
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= TELEPORTERS_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
#pragma endregion

}

void Teleporters::Render()
{
	RenderBoundingBox();
	if (vx > 0)
		nx = 1;
	else
		nx = -1;

	int ani = -1;
	if (state == TELEPORTERS_STATE_TRANSFORMING)
	{
		ani = TELEPORTERS_ANI_TRANSFORMING;
		if (animationSet->at(ani)->GetFrame() == 4)
		{
			finish_transformation = true;
		}
	}
	else if (state == TELEPORTERS_STATE_ATTACKING)
	{
		ani = TELEPORTERS_ANI_ATTACKING;
	}
	else if (state == TELEPORTERS_STATE_IDLE)
	{
		ani = TELEPORTERS_ANI_IDLE;
	}
	else if (state == TELEPORTERS_STATE_DIE) {
		ani = TELEPORTERS_ANI_DIE;
	}

	animationSet->at(ani)->Render(nx, x, y);
	//RenderBoundingBox();
}

Teleporters::Teleporters(float x, float y, LPGAMEENTITY t)
{
	SetState(TELEPORTERS_STATE_IDLE);
	enemyType = TELEPORTERS;
	tag = Tag_Teleporters;
	this->x = x;
	this->y = y;
	dam = 1;
	nx = -1;
	isAttack = 0;
	this->target = t;
	health = TELEPORTERS_MAXHEALTH;
	isActive = false;
	bbARGB = 250;
}

void Teleporters::AttackTarget(LPGAMEENTITY target) //đi theo nhân vật
{
	if (!finish_transformation)
		SetState(TELEPORTERS_STATE_TRANSFORMING);
	else SetState(TELEPORTERS_STATE_ATTACKING);
}

void Teleporters::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case TELEPORTERS_STATE_DIE:
		y += TELEPORTERS_BBOX_HEIGHT - TELEPORTERS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	//case TELEPORTERS_STATE_ATTACKING:
		/*if (nx > 0)
		{
			vx = TELEPORTERS_WALKING_SPEED;
		}
		else
		{
			vx = -TELEPORTERS_WALKING_SPEED;
		}*/
	}
}

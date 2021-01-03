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
	if (state == WORM_STATE_CLIMB)
	{
		if (abs(larvaFloor - y) < 17)
		{
			y += dy;
		}
		else
		{
			vy = 0;
			x += nx * 2.0f;
			isContainedInLarva = false;
			SetState(WORM_STATE_WALKING);
		}
	}
	else
	{
		vy += WORM_GRAVITY * dt;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		vector<LPGAMEENTITY> colliable_Objects;

		coEvents.clear();
		colliable_Objects.clear();



		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
				colliable_Objects.push_back(coObjects->at(i));
			if (coObjects->at(i)->GetType() == TAG_INJURING_BRICK && canClimbLarva == 1)
			{
				if (coObjects->at(i)->IsCollidingObject(this))
					isContainedInLarva = true;
			}
			// turn off collision when die
			if (state != WORM_STATE_DIE)
				CalcPotentialCollisions(&colliable_Objects, coEvents);
		}



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
			if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= WORM_SITEFOLLOW_PLAYER && state != WORM_STATE_CLIMB)
			{
				FollowTarget(target);
			}
			else isFollow = false;
			//Wall or reaching the edges
			if (nx != 0)
			{
				if (isContainedInLarva)
				{
					//nhay len
					SetState(WORM_STATE_CLIMB);
					isContainedInLarva = false;

				}
				else if (!isFollow)
				{
					this->nx = -this->nx;
				}

			}
			if (ny != 0 && state == WORM_STATE_WALKING && !isFollow)
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
		//clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void Worm::Render()
{
	int ani = WORM_ANI_WALKING;
	if (state == WORM_STATE_DIE) {
		ani = WORM_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			animationSet->at(ani)->ResetCurrentFrame();
			isDoneDeath = true;
		}
		animationSet->at(ani)->Render(nx, x, y  + WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE-3);
	}
	else if (state == WORM_STATE_CLIMB || state == WORM_STATE_WALKING)
		animationSet->at(ani)->Render(nx, x, y);
}

Worm::Worm(float xPos, float yPos, LPGAMEENTITY t, int canClimb)
{
	SetState(WORM_STATE_WALKING);
	enemyType = WORM;
	tag = EntityType::ENEMY;
	this->x = xPos;
	this->y = yPos;
	nx = 1;
	isFollow = false;
	this->target = t;
	health = WORM_MAXHEALTH;
	dam = 1;
	bbARGB = 250;
	isContainedInLarva = false;
	canClimbLarva = canClimb;
	this->dam = 1;
}

void Worm::FollowTarget(LPGAMEENTITY target)
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
	isFollow = true;
}

void Worm::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
		case WORM_STATE_DIE:
		{
			vx = 0;
			vy = 0;

			break;
		}
		case WORM_STATE_WALKING:
		{

			if (nx > 0)
			{
				vx = WORM_WALKING_SPEED;
			}
			else
			{
				vx = -WORM_WALKING_SPEED;
			}
			break;
		}
		case WORM_STATE_CLIMB:
		{
			vx = 0;
			vy = -WORM_CLIMB_SPEED_Y;
			larvaFloor = y;
			break;
		}
	}
}

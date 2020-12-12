#include "SkullBullet.h"
#include "global.h"
SkullBullet::SkullBullet(float posX, float posY, int direct)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_SKULL_ENEMY_BULLET));
	alpha = 255;
	bbARGB = 0;
	tag = EntityType::BULLET;
	this->SetState(BULLET_SKULL_STATE_ROLLING);
	//isHitBrick = false;
	//isHitEnemy = false;
	timeDelayed = 0;
	timeDelayedMax = BULLET_SKULL_DELAY;
	typeBullet = EntityType::SKULLS_BULLET;
	dam = 1;
	nx = direct;
	x = posX;
	y = posY;
	
	/*vy = BULLET_SKULL_BOUNCE;
	vx = BULLET_SKULL_ROLLING;*/
	isActive = true;
}

SkullBullet::~SkullBullet()
{
}

void SkullBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	
	right = left + BULLET_SKULL_BBOX_WIDTH;
	bottom = top + BULLET_SKULL_BBOX_HEIGHT;
	
}

void SkullBullet::Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects)
{
	if (!isActive)
		return;
#pragma region fall down
	vy += BULLET_SKULL_GRAVITY * dt;
#pragma region rolling friction
	vx /= BULLET_SKULL_FRICTIONAL;
#pragma region set dam and direction
	dam = 1;

	 
	this->SetState(BULLET_SKULL_STATE_ROLLING);
	if (timeDelayed >= timeDelayedMax)
	{
		isActive = false;
		timeDelayed = 0;
		return;
	}
	else if (timeDelayed >= (timeDelayedMax*0.9))
	{
		this->SetState(BULLET_SKULL_STATE_EXPLOSION);
		return;
	}
	else
	{
		timeDelayed += dt;
		Entity::Update(dt);
	}
#pragma endregion
#pragma region collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(colliable_objects, coEvents);
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
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == EntityType::TAG_BRICK)
			{
				isHitBrick = true;
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				if (!rolling) {
					vx = BULLET_SKULL_ROLLING*this->nx;
					rolling = true;
				}

				vy = -(BULLET_SKULL_BOUNCE- bounceIndex);
				bounceIndex+=0.03f;
			}
			/*if (e->obj->GetType() == EntityType::ENEMY)
			{
				e->obj->AddHealth(-dam);
				DebugOut(L"xxxxxxxxxxxxxxxx %d", e->obj->health);
				this->SetState(BULLET_SKULL_STATE_EXPLOSION);
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				isActive = false;
			}*/
			if (e->obj->GetType() == EntityType::TAG_PLAYER)
			{
				e->obj->AddHealth(-1);
				DebugOut(L"xxxxxxxxxxxxxxxx %d", e->obj->health);
				isHitEnemy = true;
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				vx = 0;
				vy = 0;
				//isActive = false;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void SkullBullet::Render()
{
	if (!isActive)
		return;

	int ani;
	DebugOut(L"SKULL bullet render");

	//if (!isHitBrick && !isHitEnemy)
	if (state == BULLET_SKULL_STATE_ROLLING)
	{

		ani = BULLET_SKULL_ANI_FALL_DOWN_AND_ROLLING;

		animationSet->at(ani)->OldRender(x, y);
	}
	if (state == BULLET_SKULL_STATE_EXPLOSION)
	{
		ani = BULLET_SKULL_ANI_EXPLOSION;
		animationSet->at(ani)->OldRender(x, y);
		if (animationSet->at(ani)->GetFrame() == 2)
		{
			isActive = false;
		}
	}
	 

}

void SkullBullet::SetState(int state)
{
	
	Entity::SetState(state);
	DebugOut(L"Bullet state: %d", state);
	switch (state)
	{
	case BULLET_SKULL_STATE_ROLLING:
	{
		isHitBrick = true;
		isHitEnemy = false;
		//vx = BULLET_SPEED *nx;
		
		isActive = true;
		break;
	}
	case BULLET_SKULL_STATE_EXPLOSION:
	{
		isHitBrick = false;
		this->dam = 1;
		break;
	}
	}
}

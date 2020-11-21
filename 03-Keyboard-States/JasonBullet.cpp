#include "JasonBullet.h"
#include "global.h"
JasonBullet::JasonBullet(float posX, float posY, int level, int direct, bool isGunFlip)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_JASON_BULLET));
	alpha = 255;
	bbARGB = 0;
	isHitBrick = false;
	isHitEnemy = false;
	dam = 1;
	timeDelayed = 0;
	timeDelayedMax = BULLET_JASON_DELAY;
	typeBullet = level;
	nx = direct;
	isAimingTop = isGunFlip;
	if (!isAimingTop)
	{
		x = posX + nx * DISTANCE_FIRING_WIDTH;
		y = posY + DISTANCE_FIRING_HEIGHT;
	}
	else
	{
		x = posX + DISTANCE_FIRING_WIDTH;
		y = posY - DISTANCE_FIRING_HEIGHT*2;
	}
	isActive = true;
}

JasonBullet::~JasonBullet()
{
}

void JasonBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (typeBullet == JASON_NORMAL_BULLET)
	{
		if (isAimingTop)
		{
			right = left + BULLET_JASON_NORMAL_VERTICAL_BBOX_WIDTH;
			bottom = top + BULLET_JASON_NORMAL_VERTICAL_BBOX_HEIGHT;
		}
		else
		{
			right = left + BULLET_JASON_NORMAL_HORIZONTAL_BBOX_WIDTH;
			bottom = top + BULLET_JASON_NORMAL_HORIZONTAL_BBOX_HEIGHT;
		}
	}
	else
	{
		if (isAimingTop)
		{
			right = left + BULLET_JASON_UPGRADE_VERTICAL_BBOX_WIDTH;
			bottom = top + BULLET_JASON_UPGRADE_VERTICAL_BBOX_HEIGHT;
		}
		else
		{
			right = left + BULLET_JASON_UPGRADE_HORIZONTAL_BBOX_WIDTH;
			bottom = top + BULLET_JASON_UPGRADE_HORIZONTAL_BBOX_HEIGHT;
		}
	}
}

void JasonBullet::Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects)
{
	if (!isActive)
		return;
#pragma region set dam and direction
	if (typeBullet == JASON_NORMAL_BULLET)
		dam = 1;
	else dam = 2;

	if (timeDelayed >= timeDelayedMax)
	{
		isActive = false;
		timeDelayed = 0;
		return;
	}
	else
	{
		timeDelayed += dt;
		Entity::Update(dt);
		if (isAimingTop == false)
		{
			vx = BULLET_SPEED * nx;
			vy = 0;
		}
		else
		{
			vy = -BULLET_SPEED;
			vx = 0;
		}
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
				vx = 0;
				vy = 0;
			}
			if (e->obj->GetType() == EntityType::ENEMY)
			{
				e->obj->AddHealth(-dam);
				DebugOut(L"xxxxxxxxxxxxxxxx %d", e->obj->health);
				isHitEnemy = true;
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				vx = 0;
				vy = 0;
				isActive = false;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void JasonBullet::Render()
{
	if (!isActive)
		return;
	RenderBoundingBox();
	int ani;
	

	if (!isHitEnemy && !isHitBrick)
	{
		if (isAimingTop)
		{
			if (typeBullet == JASON_NORMAL_BULLET)
			{
				ani = BULLET_JASON_NORMAL_ANI_TOP;
			}
			else if (typeBullet == JASON_UPGRADE_BULLET)
				ani = BULLET_JASON_UPGRADE_ANI_TOP;
			animationSet->at(ani)->OldRender(x, y, alpha);
		}
		else
		{
			if (typeBullet == JASON_NORMAL_BULLET)
				ani = BULLET_JASON_NORMAL_ANI_RIGHT;
			else if (typeBullet == JASON_UPGRADE_BULLET)
				ani = BULLET_JASON_UPGRADE_ANI_RIGHT;
			animationSet->at(ani)->Render(nx ,x, y, alpha);
		}
	}
	else if (isHitBrick)
	{
		ani = BULLET_BANG;
		if (nx == 1 && !isAimingTop)
		{
			animationSet->at(ani)->OldRender(x + BULLET_JASON_NORMAL_HORIZONTAL_BBOX_WIDTH - DISTANCE_BLOWING_UP*2, y -DISTANCE_BLOWING_UP, alpha);
		}
		else animationSet->at(ani)->OldRender(x - DISTANCE_BLOWING_UP, y - DISTANCE_BLOWING_UP, alpha);
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isActive = false;
		}
	}
	
}

void JasonBullet::SetState(int state)
{
	Entity::SetState(state);
}

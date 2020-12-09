#include "BigNavigatedEnemyBullet.h"

BigNavigatedEnemyBullet::BigNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x, int direct_y, LPGAMEENTITY t)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_ENEMY_BULLET));
	tag = EntityType::BULLET;
	alpha = 255;
	bbARGB = 0;
	isHitBrick = isHitEnemy = false;
	dam = 1;
	switch (type_enemy)
	{
		case CANNONS:
		{
			typeBullet = CANNONS_BULLET;
			break;
		}
		default:
		{
			typeBullet = BULLET;
			break;
		}
	}
	switch (typeBullet)
	{
		case CANNONS_BULLET:
		{
			bullet_speed = CANNONS_BULLET_SPEED;
			break;
		}
		default:
		{
			bullet_speed = BULLET_SPEED;
			break;
		}
	}
	nx = direct_x;
	ny = direct_y;
	isActive = true;
	x = posX;
	y = posY;
	timeDelayed = 0;
	timeDelayedMax = BIG_BULLET_ENEMY_DELAY;
	this->target = t;
	xBullet= posX;
	yBullet = posY;
	xTarget = target->x;
	yTarget = target->y;
}

BigNavigatedEnemyBullet::~BigNavigatedEnemyBullet()
{

}

void BigNavigatedEnemyBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BIG_NAVI_ENEMY_BULLET_BBOX_WIDTH;
	bottom = top + BIG_NAVI_ENEMY_BULLET_BBOX_HEIGHT;
}

void BigNavigatedEnemyBullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive)
		return;
#pragma region set dam and direction
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
		switch (typeBullet)
		{
			case CANNONS_BULLET:
			{
				if (ny == 0)
				{
					vx = bullet_speed * nx;
					vy = 0;
				}
				else
				{
					vy = bullet_speed * ny;
					vx = 0;
				}
				break;
			}
			default:
			{
				vx = bullet_speed * nx;
				if (nx == -1)
				{
					vt = -vx;
				}
				vy = bullet_speed * ny;
				//vy = CalPositionTarget(target, v) * ny;
				break;
			}
		}
	}
#pragma endregion
#pragma region collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> colliable_objects;

	coEvents.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_GATE_OVERWORLD)
			colliable_objects.push_back(coObjects->at(i));
	}
	//add target into coObjects list
	colliable_objects.push_back(target);
	CalcPotentialCollisions(&colliable_objects, coEvents);
	if (coEvents.size() == 0)
	{
		switch (typeBullet)
		{
			case CANNONS_BULLET:
			{
				x += dx;
				y += dy;
				break;
			}
			default:
			{
				x += dx;
				y = CalPositionTarget(target, x);
				break;
			}
		}
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
			if (e->obj->GetType() == EntityType::TAG_BRICK || e->obj->GetType() == EntityType::TAG_GATE_OVERWORLD)
			{
				isHitBrick = true;
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				vx = 0;
				vy = 0;
				DebugOut(L"hit brick %d", e->obj->health);

			}
			else if (e->obj->GetType() == EntityType::TAG_PLAYER)
			{
				e->obj->AddHealth(-dam);
				DebugOut(L"hit player %d", e->obj->health);
				isHitEnemy = true;
				//x += min_tx * dx + nx * 0.4f;
				//y += min_ty * dy + ny * 0.4f;
				//vx = 0;
				//vy = 0;
				isActive = false;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void BigNavigatedEnemyBullet::Render()
{
	if (!isActive)
		return;
	RenderBoundingBox();
	int ani;
	if (!isHitEnemy && !isHitBrick)
	{
		ani = BIG_NAVI_ENEMY_BULLET_ANI;
		animationSet->at(ani)->OldRender(x, y);
	}
	else if (isHitBrick)
	{
		ani = ENEMY_BULLET_BANG;
		animationSet->at(ani)->OldRender(x, y);
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isActive = false;
		}
	}
}

float BigNavigatedEnemyBullet::CalPositionTarget(LPGAMEENTITY target, float xc)
{
	float a = (float)(yTarget - yBullet) / (float)(xTarget - xBullet);
	float b = yTarget - (xTarget * a);
	return ((xc * a) + b);
}

#include "BigNavigatedEnemyBullet.h"
#include "global.h"

BigNavigatedEnemyBullet::BigNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x, int direct_y, LPGAMEENTITY t)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_ENEMY_BULLET));
	this->SetState(BIG_NAVI_ENEMY_BULLET_STATE_FLYING);
	tag = EntityType::BULLET;
	alpha = 255;
	bbARGB = 0;
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
			bullet_speed = BULLET_SPEED_OTHERS;
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
	//dynamic_cast<Player*>(target)->GetPositionCenter(xTarget, yTarget);
	xTarget = target->Getx();
	yTarget = target->Gety();
	isMoving = true;
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
				CalVelocity(vx, vy, target);
				break;
			}
		}
	}
#pragma endregion
	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == TAG_GATE || coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_GATE_OVERWORLD)
			colliable_Objects->push_back(coObjects->at(i));
	}
	colliable_Objects->push_back(target);
#pragma region collision

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(colliable_Objects, coEvents);
	if (isMoving)
	{
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
				//y = CalPositionTarget(target, x);
				y += dy;
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
				if (e->obj->GetType() == EntityType::TAG_PLAYER)
				{
					if (!isHitJason)
					{
						this->SetState(BIG_NAVI_ENEMY_BULLET_STATE_HIT_PLAYER);
						dynamic_cast<Player*>(e->obj)->EnemyBulletHitPlayer(dam);
					}
					//x += min_tx * dx + nx * 0.4f;
					//y += min_ty * dy + ny * 0.4f;
					isMoving = false;
				}
				if (e->obj->GetType() == EntityType::TAG_BRICK || e->obj->GetType() == EntityType::TAG_GATE|| e->obj->GetType() == EntityType::TAG_GATE_OVERWORLD)
				{
					this->SetState(BIG_NAVI_ENEMY_BULLET_STATE_HIT_BRICK);
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					isMoving = false;
				}
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
	if (state == BIG_NAVI_ENEMY_BULLET_STATE_FLYING)
	{
		ani = BIG_NAVI_ENEMY_BULLET_ANI;
		animationSet->at(ani)->OldRender(x, y);
	}
	else if (state == BIG_NAVI_ENEMY_BULLET_STATE_HIT_BRICK || state == BIG_NAVI_ENEMY_BULLET_STATE_HIT_PLAYER)
	{
		ani = BIG_NAVI_ENEMY_BULLET_BANG;
		animationSet->at(ani)->OldRender(x - 6, y - 6);
		if (animationSet->at(ani)->GetFrame() == 2)
		{
			isHitJason = false;
			isActive = false;
		}
	}
}

//float BigNavigatedEnemyBullet::CalPositionTarget(LPGAMEENTITY target, float xc)
//{
//	float a = (float)(yTarget - yBullet) / (float)(xTarget - xBullet);
//	float b = yTarget - (xTarget * a);
//	return ((xc * a) + b);
//}

void BigNavigatedEnemyBullet::CalVelocity(float& vx, float& vy, LPGAMEENTITY t)
{
	float d = sqrt((xBullet - xTarget) * (xBullet - xTarget) + (yBullet - yTarget) * (yBullet - yTarget));
	vx = ((xTarget - xBullet) / d) * bullet_speed;
	vy = ((yTarget - yBullet) / d) * bullet_speed;
}

void BigNavigatedEnemyBullet::SetState(int state)
{
	Entity::SetState(state);
	//DebugOut(L"Bullet state: %d", state);
	switch (state)
	{
		case BIG_NAVI_ENEMY_BULLET_STATE_HIT_BRICK:
		{
			isHitBrick = true;
			vx = 0;
			vy = 0;
			break;
		}
		case BIG_NAVI_ENEMY_BULLET_STATE_HIT_PLAYER:
		{
			isHitJason = true;
			vx = 0;
			vy = 0;
			break;
		}
		case BIG_NAVI_ENEMY_BULLET_STATE_FLYING:
		{
			isActive = true;
			isHitJason = false;
			isHitBrick = false;
			break;
		}
	}
}
//mmmm
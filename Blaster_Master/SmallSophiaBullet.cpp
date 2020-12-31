#include "SmallSophiaBullet.h"
SmallSophiaBullet::SmallSophiaBullet(float posX, float posY, int level, int direct)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_SMALL_SOPHIA_BULLET));
	alpha = 255;
	bbARGB = 0;
	isAimingTop = false;
	tag = EntityType::BULLET;
	this->SetState(BULLET_STATE_FLYING);
	//isHitBrick = false;
	//isHitEnemy = false;
	dam = 1;
	timeDelayed = 0;
	timeDelayedMax = BULLET_SMALL_SOPHIA_DELAY;
	if (level == 0)
		typeBullet = EntityType::SMALL_SOPHIA_NORMAL_BULLET;
	//else typeBullet = EntityType::JASON_UPGRADE_BULLET;
	nx = direct;
	if (!isAimingTop)
	{
		x = posX + nx * DISTANCE_FIRING_WIDTH;
		y = posY + DISTANCE_FIRING_HEIGHT;
	}
	else
	{
		x = posX + DISTANCE_FIRING_WIDTH;
		y = posY - DISTANCE_FIRING_HEIGHT * 2;
	}

	//isActive = true;
}

SmallSophiaBullet::~SmallSophiaBullet()
{
}

void SmallSophiaBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (typeBullet == SMALL_SOPHIA_NORMAL_BULLET)
	{
		if (isAimingTop)
		{
			right = left + BULLET_SMALL_SOPHIA_NORMAL_VERTICAL_BBOX_WIDTH;
			bottom = top + BULLET_SMALL_SOPHIA_NORMAL_VERTICAL_BBOX_HEIGHT;
		}
		else
		{
			right = left + BULLET_SMALL_SOPHIA_NORMAL_HORIZONTAL_BBOX_WIDTH;
			bottom = top + BULLET_SMALL_SOPHIA_NORMAL_HORIZONTAL_BBOX_HEIGHT;
		}
	}
	
}

void SmallSophiaBullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive)
		return;
#pragma region set dam and direction
	if (typeBullet == SMALL_SOPHIA_NORMAL_BULLET)
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
	HandlePlayerBulletCollision(coObjects);

	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;
	//coEvents.clear();
	//CalcPotentialCollisions(colliable_objects, coEvents);
	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];
	//		if (e->obj->GetType() == EntityType::TAG_BRICK)
	//		{
	//			this->SetState(BULLET_SMALL_SOPHIA_STATE_HIT_BRICK);
	//			x += min_tx * dx + nx * 0.4f;
	//			y += min_ty * dy + ny * 0.4f;
	//			//vx = 0;
	//			//vy = 0;
	//			//isHitBrick = true;
	//		}
	//		else if (e->obj->GetType() == EntityType::ENEMY)
	//		{
	//			e->obj->AddHealth(-dam);
	//			DebugOut(L"xxxxxxxxxxxxxxxx %d", e->obj->health);
	//			this->SetState(BULLET_SMALL_SOPHIA_STATE_HIT_ENEMY);
	//			//isHitEnemy = true;
	//			x += min_tx * dx + nx * 0.4f;
	//			y += min_ty * dy + ny * 0.4f;
	//			//vx = 0;
	//			//vy = 0;
	//			isActive = false;
	//		}
	//	}
	//}
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void SmallSophiaBullet::Render()
{
	if (!isActive)
		return;
	RenderBoundingBox();
	int ani;
	DebugOut(L"Small Sophia bullet render");

	//if (!isHitBrick && !isHitEnemy)
	if (state == BULLET_STATE_FLYING)
	{
		if (typeBullet == SMALL_SOPHIA_NORMAL_BULLET)
			ani = BULLET_SMALL_SOPHIA_NORMAL_ANI_RIGHT;
		//else if (typeBullet == JASON_UPGRADE_BULLET)
		//	ani = BULLET_SMALL_SOPHIA_UPGRADE_ANI_RIGHT;
		animationSet->at(ani)->Render(nx, x, y, alpha);
		
	}
	else if (state == BULLET_STATE_HIT_BRICK)
	{
		ani = BULLET_SMALL_SOPHIA_NORMAL_ANI_BANG;
		if (nx == 1 && !isAimingTop)
		{
			animationSet->at(ani)->OldRender(x + BULLET_SMALL_SOPHIA_NORMAL_HORIZONTAL_BBOX_WIDTH - DISTANCE_BLOWING_UP*3, y - DISTANCE_BLOWING_UP, alpha);
		}
		else animationSet->at(ani)->OldRender(x - DISTANCE_BLOWING_UP, y - DISTANCE_BLOWING_UP, alpha);
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			isActive = false;
		}
	}

}

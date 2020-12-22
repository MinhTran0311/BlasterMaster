#include "JasonBullet.h"
#include "global.h"
JasonBullet::JasonBullet(float posX, float posY, int level, int direct, bool isGunFlip)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_JASON_BULLET));
	alpha = 255;
	bbARGB = 0;
	tag = EntityType::BULLET;
	this->SetState(BULLET_STATE_FLYING);
	dam = 1;
	timeDelayed = 0;
	timeDelayedMax = BULLET_JASON_DELAY;
	if (level == 0)
	{
		dam = 1;
		typeBullet = EntityType::JASON_NORMAL_BULLET;
	}
	else {
		dam = 2;
		typeBullet = EntityType::JASON_UPGRADE_BULLET;
	}
	nx = direct;
	isAimingTop = isGunFlip;
	if (!isAimingTop)
	{
		x = posX + nx * DISTANCE_FIRING_WIDTH;
		y = posY + DISTANCE_FIRING_HEIGHT;
	}
	else
	{
		x = posX + 2 * DISTANCE_FIRING_WIDTH;
		y = posY - DISTANCE_FIRING_HEIGHT*2;
	}
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

void JasonBullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive || state == BULLET_STATE_HIT_BRICK)
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
	HandlePlayerBulletCollision(coObjects);

#pragma endregion
//	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
//	for (int i = 0; i < coObjects->size(); i++)
//	{
//		if (coObjects->at(i)->GetType() == TAG_GATE || coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_SOFT_BRICK || coObjects->at(i)->GetType() == ENEMY)
//			colliable_Objects->push_back(coObjects->at(i));
//	}
//	
//#pragma region collision
//	vector<LPCOLLISIONEVENT> coEvents;
//	vector<LPCOLLISIONEVENT> coEventsResult;
//
//	coEvents.clear();
//
//	CalcPotentialCollisions(colliable_Objects, coEvents);
//	if (coEvents.size() == 0)
//	{
//		x += dx;
//		y += dy;
//	}
//	else
//	{
//		float min_tx, min_ty, nx = 0, ny;
//		float rdx = 0;
//		float rdy = 0;
//
//		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
//		for (UINT i = 0; i < coEventsResult.size(); i++)
//		{
//			LPCOLLISIONEVENT e = coEventsResult[i];
//			if (e->obj->GetType() == EntityType::TAG_BRICK || e->obj->GetType() == EntityType::TAG_GATE)
//			{
//				this->SetState(BULLET_JASON_STATE_HIT_BRICK);
//				x += min_tx * dx + nx * 0.4f;
//				y += min_ty * dy + ny * 0.4f;
//				//vx = 0;
//				//vy = 0;
//				//isHitBrick = true;
//			}
//			else if (e->obj->GetType() == EntityType::ENEMY || e->obj->GetType() == EntityType::TAG_SOFT_BRICK)
//			{
//				e->obj->AddHealth(-dam);
//				DebugOut(L"xxxxxxxxxxxxxxxxhitEnemy %d", e->obj->health);
//				this->SetState(BULLET_JASON_STATE_HIT_ENEMY);
//				//isHitEnemy = true;
//				x += min_tx * dx + nx * 0.4f;
//				y += min_ty * dy + ny * 0.4f;
//				//vx = 0;
//				//vy = 0;
//				isActive = false;
//			}
//		}
//	}
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void JasonBullet::Render()
{
	if (!isActive)
		return;
	RenderBoundingBox();
	int ani;
	//DebugOut(L"Jason bullet render");

	//if (!isHitBrick && !isHitEnemy)
	if (state== BULLET_STATE_FLYING)
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
	else if (state== BULLET_STATE_HIT_BRICK)
	{
		ani = BULLET_BANG;
		if (nx == 1 && !isAimingTop)
		{
			if (typeBullet == JASON_NORMAL_BULLET)
				animationSet->at(ani)->Render(1, x - BULLET_JASON_NORMAL_HORIZONTAL_BBOX_WIDTH, y - BULLET_JASON_NORMAL_HORIZONTAL_BBOX_HEIGHT);
			else if (typeBullet == JASON_UPGRADE_BULLET)
				animationSet->at(ani)->Render(1, x  - BULLET_JASON_UPGRADE_HORIZONTAL_BBOX_WIDTH, y - BULLET_JASON_UPGRADE_HORIZONTAL_BBOX_HEIGHT);
		}
		else animationSet->at(ani)->Render(1, x - BULLET_JASON_UPGRADE_VERTICAL_BBOX_WIDTH, y - DISTANCE_BLOWING_UP);
		if (animationSet->at(ani)->GetFrame() == 2)
		{
			isActive = false;
		}
	}
	
}

//void JasonBullet::SetState(int state)
//{
//	Entity::SetState(state);
//	//DebugOut(L"Bullet state: %d", state);
//	switch (state)
//	{
//	case BULLET_JASON_STATE_FLYING:
//	{
//		isHitBrick = false;
//		isHitEnemy = false;
//		isActive = true;
//		break;
//	}
//	case BULLET_JASON_STATE_HIT_BRICK:
//	{
//		vx = 0;
//		vy = 0;
//		isHitBrick = true;
//	}
//	case BULLET_JASON_STATE_HIT_ENEMY:
//	{
//		vx = 0;
//		vy = 0;
//		isHitEnemy = true;
//		break;
//	}
//	}
//}

#include "BossBullet.h"
#include "Player.h"


BossBullet::BossBullet(float xPos, float yPos, LPGAMEENTITY t)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BOSS_ENEMY_BULLET));
	tag = EntityType::BULLET;
	isHitEnemy = false;
	dam = 1;
	this->SetState(BOSS_ENEMY_BULLET_STATE_FLYING);
	typeBullet = BOSS_BULLET;
	bullet_speed = BOSS_BULLET_SPEED;
	x = xPos;
	y = yPos;
	if (t->Getx() > x) nx = 1;
	else if (t->Getx() < x) nx = -1;
	else nx = 0;

	ny = 1;
	timeDelayed = 0;
	timeDelayedMax = BOSS_ENEMY_DELAY;
	target = t;
	x_Start_Pos = xPos;
	y_Start_Pos = yPos;
	xTarget = target->Getx();
	yTarget = target->Gety();
	isMoving = true;
	isActive = true;
}

BossBullet::~BossBullet()
{
}

void BossBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BOSS_ENEMY_BULLET_BBOX_WIDTH;
	bottom = top + BOSS_ENEMY_BULLET_BBOX_HEIGHT;
}

void BossBullet::Update(DWORD dt, vector<LPGAMEENTITY>* co_Objects)
{
	if (!isActive || state != BOSS_ENEMY_BULLET_STATE_FLYING)
	{
		return;
	}
	if (timeDelayed >= timeDelayedMax)
	{
		isActive = false;
		timeDelayed = 0;
		return;
	}
	else
	{
		timeDelayed += dt;
		vx = bullet_speed * nx;
	}
	Entity::Update(dt);

	for (int i = 0; i < co_Objects->size(); i++)
	{
		if (co_Objects->at(i)->GetType() == TAG_PLAYER && this->IsCollidingObject(co_Objects->at(i)))
		{
			DebugOut(L"hit player");
			dynamic_cast<Player*>(co_Objects->at(i))->SetInjured(dam);
			isActive = false;
			isMoving = false;
			this->SetState(BOSS_ENEMY_BULLET_STATE_HIT_PLAYER);
			return;
		}
			
	}
	x += dx;
	y = CalPositionTarget(target, x);
	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;
	//vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	//for (int i = 0; i < co_Objects->size(); i++)
	//{
	//	if (co_Objects->at(i)->GetType() == TAG_PLAYER)
	//		colliable_Objects->push_back(co_Objects->at(i));
	//}
	//coEvents.clear();
	//CalcPotentialCollisions(colliable_Objects, coEvents);
	//if (isMoving)
	//{

	//	if (coEvents.size() == 0)
	//	{
	//		x += dx;
	//		y = CalPositionTarget(target, x);
	//	}
	//	else
	//	{
	//		float min_tx, min_ty, nx = 0, ny;
	//		float rdx = 0;
	//		float rdy = 0;
	//		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	//		for (UINT i = 0; i < coEventsResult.size(); i++)
	//		{
	//			LPCOLLISIONEVENT e = coEventsResult[i];
	//			if (e->obj->GetType() == TAG_PLAYER)
	//			{
	//				this->SetState(BOSS_ENEMY_BULLET_STATE_HIT_PLAYER);
	//				dynamic_cast<Player*>(e->obj)->SetInjured(dam);
	//			}
	//			x += min_tx * dx + nx * 0.4f;
	//			y += min_ty * dy + ny * 0.4f;
	//		}
	//	}
	//}
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BossBullet::Render()
{
	if (!isActive)
		return;
	int ani;
	if (state == BOSS_ENEMY_BULLET_STATE_FLYING)
	{
		ani = BOSS_ENEMY_BULLET_STATE_FLYING;
		animationSet->at(ani)->Render(1, x, y);
	}
	else isActive = false;
}

float BossBullet::CalPositionTarget(LPGAMEENTITY target, float xPos)
{
	float a = (float)(yTarget - y_Start_Pos) / (float)(xTarget - x_Start_Pos);
	float b = yTarget - (xTarget * a);
	return ((xPos * a) + b);
}

void BossBullet::SetState(int state)
{
	Bullet::SetState(state);
}

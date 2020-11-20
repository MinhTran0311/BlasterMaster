#include "JasonRocket.h"
#include "Bullet.h"
#include "Animations.h"
#include <algorithm>

JasonRocket::JasonRocket(float xPos, float yPos)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_JASON_ROCKET));
	alpha = 255;
	isHitEnemy = false;
	isActive = true;
	nx = 1;
	SetState(BULLET_STATE_FLYING);
	x = xPos;
	y = yPos;
	dam = 1;
}

JasonRocket::~JasonRocket()
{
}

void JasonRocket::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ROCKET_BBOX_WIDTH;
	bottom = y + ROCKET_BBOX_HEIGHT;
}

void JasonRocket::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive)
		return;
	Entity::Update(dt);
	int check = 0;
	objects = coObjects;
	for (int i = 0; i < objects->size(); i++) {
		if (objects->at(i)->GetType()==EntityType::ENEMY)
			check++;
	}
	if (check == 0)
	{
		return;
	}
	if (identifyTarget(objects))
	{
		SetNavigation();
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else {
			float min_tx, min_ty, nx = 0, ny;

			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			x += dx*dt;
			y += dy*dt;

			//if (nx != 0) vx = 0;
			//if (ny != 0) vy = 0.00f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->obj->GetType() == EntityType::ENEMY)
				{
					e->obj->AddHealth(-dam);
					DebugOut(L"xxxxxxxxxxxxxxxx %d", e->obj->health);
					isHitEnemy = true;
					/*x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;*/

					isActive = false;
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	else
	{
		isActive = false;
	}
}

void JasonRocket::Render()
{
	if (!isActive)
		return;
	int ani;
	if (state == BULLET_STATE_FLYING)
		ani = ROCKET_ANI_FLYING;
	animationSet->at(ani)->Render(nx, x, y);
}

float JasonRocket::GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target)
{
	return (float)sqrt((pos.x - target.x) * (pos.x - target.x) + (pos.y - target.y) * (pos.y - target.y));

}

bool JasonRocket::identifyTarget(vector<LPGAMEENTITY>* objects)
{

	Entity* target = FirstTarget(objects);
	if (target != NULL)
	{

		float min_dis = GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->Getx(), target->Gety()));
		for (int i = 0; i < objects->size(); i++)
		{
			if (objects->at(i)->GetType() == EntityType::ENEMY)
			{
				if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(objects->at(i)->Getx(), objects->at(i)->Gety())) < min_dis)
				{
					target = objects->at(i);
				}
			}
		}
		SetTargetObject(target);
		return true;
	}
	else return false;
}

//void JasonRocket::setTargetObjects(vector<LPGAMEENTITY>* objects)
//{
//	this->objects = objects;
//}

Entity* JasonRocket::FirstTarget(vector<LPGAMEENTITY>* objects)
{
	for (int i = 0; i < objects->size(); i++)
	{
		if (objects->at(i)->GetType() == EntityType::ENEMY)
		{
			if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(objects->at(i)->Getx(), objects->at(i)->Gety())) < MAX_FLYING_RANGE)
			{
				return objects->at(i);
			}
		}
	}
	
}

void JasonRocket::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case BULLET_STATE_BLOW_UP:
		vx = 0;
		vy = 0;
	default:
		break;
	}
}

void JasonRocket::SetNavigation()
{
	targetObject->GetPosition(targetPos.first, targetPos.second);
	if (targetPos.first > x) {
		if (targetPos.second > y) {
			vx = ROCKET_FLYING_SPEED;
			vy = ROCKET_FLYING_SPEED;
			nx = 1;
			SetState(BULLET_STATE_FLYING);
		}
		else
		{
			vx = ROCKET_FLYING_SPEED;
			vy = -ROCKET_FLYING_SPEED;
			nx = 1;
			SetState(BULLET_STATE_FLYING);
		}
	}
	else
	{
		if (targetPos.second > y) {
			vx = -ROCKET_FLYING_SPEED;
			vy = ROCKET_FLYING_SPEED;
			nx = -1;
			SetState(BULLET_STATE_FLYING);
		}
		else
		{
			vx = -ROCKET_FLYING_SPEED;
			vy = -ROCKET_FLYING_SPEED;
			nx = -1;
			SetState(BULLET_STATE_FLYING);
		}
	}
}

void JasonRocket::SetTargetObject(Entity* object)
{
	this->targetObject = &(*object);
}

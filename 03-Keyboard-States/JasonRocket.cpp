#include "JasonRocket.h"
#include "Bullet.h"
#include "Animations.h"
#include <algorithm>

JasonRocket::JasonRocket(float xPos, float yPos, int direct_x)
{
	tag = EntityType::BULLET;
	typeBullet = EntityType::JASON_ROCKET_BULLET;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_JASON_ROCKET));
	alpha = 255;
	isHitEnemy = false;
	isActive = true;
	nx = direct_x;
	SetState(BULLET_JASON_STATE_FLYING);
	x = xPos;
	y = yPos;
	dam = 1;
	timeDelayedMax = BULLET_JASON_DELAY;
	DebugOut(L"create rocket");
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
	
	timeDelayed += dt;

	int check = 0;
	//objects = coObjects;
	//for (int i = 0; i < objects->size(); i++) {
	//	if (objects->at(i)->GetType()==EntityType::ENEMY)
	//		check++;
	//}

	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == ENEMY)
		{
			colliable_Objects->push_back(coObjects->at(i));
			check++;
		}
	}

	if (check == 0 )
	{
		if (timeDelayed >= timeDelayedMax)
		{
			isActive = false;
			timeDelayed = 0;
		}
		else
		{
			x += nx * ROCKET_FLYING_SPEED*dt;
		}
		return;
	}
	else if (identifyTarget(colliable_Objects))
	{
		SetNavigation();
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		//code lau
		for (int i = 0; i < colliable_Objects->size(); i++)
		{
			if (colliable_Objects->at(i)->IsCollidingObject(this))
			{
				colliable_Objects->at(i)->AddHealth(-dam);
				DebugOut(L"xxxxxxxxxxxxxxxx %d", colliable_Objects->at(i)->health);
				this->SetState(BULLET_JASON_STATE_HIT_ENEMY);
				//isHitEnemy = true;
				//x += min_tx * dx + nx * 0.4f;
				//y += min_ty * dy + ny * 0.4f;

				isActive = false;
			}
		}

		CalcPotentialCollisions(colliable_Objects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else 
		{
			float min_tx, min_ty, nx = 0, ny=0;

			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			//if (nx != 0) vx = 0;
			//if (ny != 0) vy = 0.00f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (e->obj->GetType() == EntityType::ENEMY)
				{
					e->obj->AddHealth(-dam);
					DebugOut(L"xxxxxxxxxxxxxxxx %d", e->obj->health);
					this->SetState(BULLET_JASON_STATE_HIT_ENEMY);
					//isHitEnemy = true;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;

					isActive = false;
				}
			}
			//x += dx * dt;
			//y += dy * dt;
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
	if (state == BULLET_JASON_STATE_FLYING)
		ani = ROCKET_ANI_FLYING;
	animationSet->at(ani)->Render(nx, x, y);
}

float JasonRocket::GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target)
{
	return (float)sqrt((pos.x - target.x) * (pos.x - target.x) + (pos.y - target.y) * (pos.y - target.y));

}

bool JasonRocket::identifyTarget(vector<LPGAMEENTITY>* objects)
{
	LPGAMEENTITY target = FirstTarget(objects);
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

LPGAMEENTITY JasonRocket::FirstTarget(vector<LPGAMEENTITY>* objects)
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
	return nullptr;
}

void JasonRocket::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case BULLET_JASON_STATE_HIT_ENEMY:
	{
		vx = 0;
		vy = 0;
		isHitEnemy = true;
		break;
	}
	case BULLET_JASON_STATE_FLYING:
	{
		isHitBrick = false;
		isHitEnemy = false;
		isActive = true;
		break;
	}
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
			SetState(BULLET_JASON_STATE_FLYING);
		}
		else
		{
			vx = ROCKET_FLYING_SPEED;
			vy = -ROCKET_FLYING_SPEED;
			nx = 1;
			SetState(BULLET_JASON_STATE_FLYING);
		}
	}
	else
	{
		if (targetPos.second > y) {
			vx = -ROCKET_FLYING_SPEED;
			vy = ROCKET_FLYING_SPEED;
			nx = -1;
			SetState(BULLET_JASON_STATE_FLYING);
		}
		else
		{
			vx = -ROCKET_FLYING_SPEED;
			vy = -ROCKET_FLYING_SPEED;
			nx = -1;
			SetState(BULLET_JASON_STATE_FLYING);
		}
	}
}

void JasonRocket::SetTargetObject(Entity* object)
{
	this->targetObject = &(*object);
}

#include "SkullBullet.h"
#include "global.h"
#include "Player.h"
SkullBullet::SkullBullet(float posX, float posY, int direct, LPGAMEENTITY t)
{
	this->target = t;
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

void SkullBullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive)
		return;
#pragma region fall down
	vy += BULLET_SKULL_GRAVITY * dt;
#pragma region rolling friction
	vx /= BULLET_SKULL_FRICTIONAL;
#pragma region set dam and direction
	dam = 1;
	if (isHitEnemy) {

		this->SetState(BULLET_SKULL_STATE_EXPLOSION);
		Sound::GetInstance()->Play("EnemyBulletBang", 0, 1);
	}
	 
	if (timeDelayed >= timeDelayedMax)
	{
		isActive = false;
		timeDelayed = 0;
		return;
	}
	else if (timeDelayed >= (timeDelayedMax*0.9))
	{
		this->SetState(BULLET_SKULL_STATE_EXPLOSION);
		Sound::GetInstance()->Play("EnemyBulletBang", 0, 1);
		return;
	}
	else
	{
		timeDelayed += dt;
		Entity::Update(dt);
	}
#pragma endregion
	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == TAG_GATE || coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_PLAYER)
			colliable_Objects->push_back(coObjects->at(i));
	}
	colliable_Objects->push_back(target);
#pragma region collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	
	//for (int i = 0; i < coObjects->size(); i++)
	//{
	//	if (this->IsCollidingObject(coObjects->at(i)) && coObjects->at(i)->GetType() == TAG_PLAYER)
	//	{
	//			coObjects->at(i)->AddHealth(-1);
	//			DebugOut(L"xxxxxxxxxxxxxxxx %d", coObjects->at(i)->health);
	//			isHitEnemy = true;
	//			SetState(BULLET_SKULL_STATE_EXPLOSION);
	//			vx = 0;
	//			vy = 0;
	//			//isActive = false;
	//	}
	//}

	//vector<LPGAMEENTITY>* colliable_objects = new vector<LPGAMEENTITY>();

	//for (int i = 0; i < coObjects->size(); i++)
	//{
	//	if (coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_GATE)
	//		colliable_objects->push_back(coObjects->at(i));
	//}
	coEvents.clear();
	CalcPotentialCollisions(colliable_Objects, coEvents);
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
			if (e->obj->GetType() == EntityType::TAG_BRICK || e->obj->GetType() == EntityType::TAG_GATE)
			{
				isHitBrick = true;
				x += min_tx * dx + nx * 1.0f;
				y += min_ty * dy + ny * 1.0f;

				if (!rolling) {
					vx = BULLET_SKULL_ROLLING * this->nx;
					rolling = true;
				}

				vy = -(BULLET_SKULL_BOUNCE- bounceIndex);
				bounceIndex += 0.03f;
			}

			if (e->obj->GetType() == EntityType::TAG_PLAYER)
			{
				dynamic_cast<Player*>(e->obj)->EnemyBulletHitPlayer(dam);
				DebugOut(L"Dinh target1 %d", e->obj->health);
				isHitEnemy = true;
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
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
	else if (state == BULLET_SKULL_STATE_EXPLOSION)
	{
		DebugOut(L"Dinh target2");
		ani = BULLET_SKULL_ANI_EXPLOSION;
		DebugOut(L"Dinh target3 %d", ani);
		animationSet->at(ani)->OldRender(x, y);
		if (animationSet->at(ani)->GetFrame() == 2)
		{
			DebugOut(L"Dinh target4");
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
			vx = 0;
			vy = 0;
			isHitBrick = false;
			break;
		}
	}
}

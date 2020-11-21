#include "BigNavigatedEnemyBullet.h"

BigNavigatedEnemyBullet::BigNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x = 0, int direct_y = 0)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_ENEMY_BULLET));
	alpha = 255;
	bbARGB = 0;
	isHitBrick = isHitEnemy = false;
	dam = 1;
	typeBullet = type_enemy;
	nx = direct_x;
	ny = direct_y;
	isActive = true;
	x = posX;
	y = posY;
	timeDelayed = 0;
	timeDelayedMax = BIG_BULLET_ENEMY_DELAY;
	targetObject = nullptr;
}

//BigNavigatedEnemyBullet::BigNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x = 0, int direct_y = 0, LPGAMEENTITY target)
//{
//	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_ENEMY_BULLET));
//	alpha = 255;
//	bbARGB = 0;
//	isHitBrick = isHitEnemy = false;
//	dam = 1;
//	typeBullet = type_enemy;
//	nx = direct_x;
//	ny = direct_y;
//	isActive = true;
//	x = posX;
//	y = posY;
//	timeDelayed = 0;
//	timeDelayedMax = BIG_BULLET_ENEMY_DELAY;
//	targetObject = target;
//	CalcDistanceEquation(equation_a, equation_b, targetObject->Getx(), targetObject->Gety(), x, y);
//}

void BigNavigatedEnemyBullet::CalcDistanceEquation(float& equa_a, float& equa_b, float target_x, float target_y, float x, float y)
{
	equa_a = (target_y - y) / (target_x - x);
	equa_b = (target_y - (target_x * equa_a));
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

void BigNavigatedEnemyBullet::Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects)
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
		case EntityType::TAG_CANNONS:
			bullet_speed = CANNONS_BULLET_SPEED;
			break;
		default:
			bullet_speed = BULLET_SPEED;
			break;
		}
		//if (ny==0)
		//{
		//	vx = BULLET_SPEED * nx;
		//	vy = 0;
		//}
		//else if (nx==0)
		//{
		//	vy = BULLET_SPEED*ny;
		//	vx = 0;
		//}
		//else
		//{
		//	vx= BULLET_SPEED * nx;
		//	vy = BULLET_SPEED * ny;
		//}
		switch (typeBullet)
		{
			case EntityType::TAG_CANNONS:
				vx = CANNONS_BULLET_SPEED * nx;
				vy = CANNONS_BULLET_SPEED * ny;
				break;
			default:
				vx = BULLET_SPEED * nx;
				vy = BULLET_SPEED * ny; //Không cần thiết
				break;
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
		switch (typeBullet)
		{
		case EntityType::TAG_CANNONS:
			x += dx;
			y += dy;
			break;
		default:
			x += dx;
			y = x * equation_a + equation_b;
			break;
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
			if (e->obj->GetType() == EntityType::TAG_BRICK)
			{
				isHitBrick = true;
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				vx = 0;
				vy = 0;
			}
			if (e->obj->GetType() == EntityType::TAG_JASON || e->obj->GetType() == EntityType::TAG_SMALL_SOPHIA || e->obj->GetType() == EntityType::TAG_BIG_SOPHIA)
			{
				e->obj->AddHealth(-dam);
				DebugOut(L"xxxxxxxxxxxxxxxx %d\n", e->obj->health);
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

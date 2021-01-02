#include "SmallNavigatedEnemyBullet.h"
#include "global.h"
#include "Player.h"

SmallNavigatedEnemyBullet::SmallNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x, int direct_y, LPGAMEENTITY t)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_SMALL_ENEMY_BULLET));
	this->SetState(SMALL_NAVI_ENEMY_BULLET_STATE_FLYING);
	tag = EntityType::BULLET;
	alpha = 255;
	bbARGB = 0;
	dam = 1;
	switch (type_enemy)
	{
		case MINES:
		{
			typeBullet = MINES_BULLET;
			timeDelayedMax = SMALL_BULLET_ENEMY_DELAY * 2;
			vy = -BULLET_SPEED_MINES;
			timeFlyUp->Start();
			vx = RandomVx();
			break;
		}
		default:
		{
			typeBullet = BULLET;
			timeDelayedMax = SMALL_BULLET_ENEMY_DELAY;
			break;
		}
	}
	bullet_speed = BULLET_SPEED_OTHERS;
	nx = direct_x;
	ny = direct_y;
	isActive = true;
	x = posX;
	y = posY;
	timeDelayed = 0;
	this->target = t;
	xBullet = posX;
	yBullet = posY;
	isMoving = true;
	//dynamic_cast<Player*>(target)->GetPositionCenter(xTarget, yTarget);
	xTarget = target->Getx();
	yTarget = target->Gety();
}

SmallNavigatedEnemyBullet::~SmallNavigatedEnemyBullet()
{

}

void SmallNavigatedEnemyBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + SMALL_NAVI_ENEMY_BULLET_BBOX_WIDTH;
	bottom = top + SMALL_NAVI_ENEMY_BULLET_BBOX_HEIGHT;
}

void SmallNavigatedEnemyBullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
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
			case MINES_BULLET:
			{
				if (timeFlyUp->IsTimeUp())
				{
					if (vy < BULLET_SPEED_MINES)
					{
						vy += BULLET_SPEED_MINES/10;
					}
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
		if (coObjects->at(i)->GetType() == TAG_GATE || coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_PLAYER)
			colliable_Objects->push_back(coObjects->at(i));
	}
#pragma region collision
	colliable_Objects->push_back(target);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(colliable_Objects, coEvents);
	if (isMoving)
	{
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
				if (e->obj->GetType() == EntityType::TAG_PLAYER)
				{
					if (dynamic_cast<Player*>(e->obj)->IsImmortaling())
					{
						x += dx;
						y += dy;
					}
					else if (isMoving)
					{
						if (!isHitJason)
						{
							isHitJason = true;
							dynamic_cast<Player*>(e->obj)->EnemyBulletHitPlayer(dam);
						}
						x += min_tx * dx + nx * 0.4f;
						y += min_ty * dy + ny * 0.4f;
						this->SetState(SMALL_NAVI_ENEMY_BULLET_STATE_HIT_PLAYER);
						isMoving = false;
					}
				}
				if (e->obj->GetType() == EntityType::TAG_BRICK || e->obj->GetType() == EntityType::TAG_GATE)
				{
					if (isMoving)
					{
						switch (typeBullet)
						{
							case MINES_BULLET:
							{
								x += dx;
								y += dy;
								break;
							}
							default:
							{
								this->SetState(SMALL_NAVI_ENEMY_BULLET_STATE_HIT_BRICK);
								Sound::GetInstance()->Play("EnemyBulletBang", 0, 1);
								x += min_tx * dx + nx * 0.4f;
								y += min_ty * dy + ny * 0.4f;
								isMoving = false;
								break;
							}
						}
					}
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void SmallNavigatedEnemyBullet::Render()
{
	if (!isActive)
		return;
	RenderBoundingBox();
	int ani;
	if (state == SMALL_NAVI_ENEMY_BULLET_STATE_FLYING)
	{
		ani = SMALL_NAVI_ENEMY_BULLET_ANI;
		animationSet->at(ani)->OldRender(x, y);
	}
	else if (state == SMALL_NAVI_ENEMY_BULLET_STATE_HIT_BRICK || state == SMALL_NAVI_ENEMY_BULLET_STATE_HIT_PLAYER)
	{
		ani = SMALL_NAVI_ENEMY_BULLET_BANG;
		animationSet->at(ani)->OldRender(x - 6, y - 6);
		if (animationSet->at(ani)->GetFrame() == animationSet->at(ani)->GetLastFrameIndex())
		{
			isActive = false;
			isHitJason = false;
		}
	}
}

//float SmallNavigatedEnemyBullet::CalPositionTarget(LPGAMEENTITY target, float xc)
//{
//	float a = (float)(yTarget - yBullet) / (float)(xTarget - xBullet);
//	float b = yTarget - (xTarget * a);
//	return ((xc * a) + b);
//}

void SmallNavigatedEnemyBullet::CalVelocity(float& vx, float& vy, LPGAMEENTITY t)
{
	float d = sqrt((xBullet - xTarget) * (xBullet - xTarget) + (yBullet - yTarget) * (yBullet - yTarget));
	vx = ((xTarget - xBullet) / d) * bullet_speed;
	vy = ((yTarget - yBullet) / d) * bullet_speed;
}

float SmallNavigatedEnemyBullet::RandomVx()
{
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<float> dis(-0.04, 0.04);
	return dis(mt);
}

void SmallNavigatedEnemyBullet::SetState(int state)
{
	Entity::SetState(state);
	//DebugOut(L"Bullet state: %d", state);
	switch (state)
	{
		case SMALL_NAVI_ENEMY_BULLET_STATE_HIT_BRICK:
		{
			isHitBrick = true;
			vx = 0;
			vy = 0;
			break;
		}
		case SMALL_NAVI_ENEMY_BULLET_STATE_HIT_PLAYER:
		{
			vx = 0;
			vy = 0;
			break;
		}
		case SMALL_NAVI_ENEMY_BULLET_STATE_FLYING:
		{
			isActive = true;
			isHitJason = false;
			isHitBrick = false;
			break;
		}
	}
}

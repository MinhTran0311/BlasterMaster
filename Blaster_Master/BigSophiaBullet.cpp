#include "BigSophiaBullet.h"

BigSophiaBullet::BigSophiaBullet(float posX, float posY, int level, int directionX, int directionY)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_SOPHIA_BULLET));
	alpha = 255;
	bbARGB = 0;
	tag = BULLET;
	this->SetState(BULLET_STATE_FLYING);

	timeDelayed = 0;


	nx = directionX;
	ny = directionY;
	if (level == 1)
	{
		typeBullet = BIG_SOPHIA_BULLET_LV1;
		dam = 1;
		timeDelayedMax = BULLET_BIG_SOPHIA_LV1_DELAY;
	}
	else if (level == 2)
	{
		typeBullet = BIG_SOPHIA_BULLET_LV2;
		dam = 1;
		if (nx == 1) angle = 0;
		else if (nx == -1)angle = 180;
		else if (ny == 1) angle = 90;
		else if (ny == -1)angle = 270;
		timeDelayedMax = BULLET_BIG_SOPHIA_DELAY;
	}
	else
	{
		typeBullet = BIG_SOPHIA_BULLET_LV3;
		dam = 2;
		angle = 0;
		timeDelayedMax = BULLET_BIG_SOPHIA_DELAY;
	}
	if (nx == 0)
	{
		x = posX + 7;
		if (ny == -1) y = posY;
		else y = posY + 32;
	}
	else if (ny == 0)
	{
		y = posY + 12;
		if (nx == 1) x = posX + 22;
		else x = posX;
	}
	startingPoint.first = x;
	startingPoint.second = y;
}

BigSophiaBullet::~BigSophiaBullet()
{
}

void BigSophiaBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (ny != 0)
	{
		right = left + BULLET_BIG_SOPHIA_VERTICAL_BBOX_WIDTH;
		bottom = top + BULLET_BIG_SOPHIA_VERTICAL_BBOX_HEIGHT;
	}
	else
	{
		right = left + BULLET_BIG_SOPHIA_HORIZONTAL_BBOX_WIDTH;
		bottom = top + BULLET_BIG_SOPHIA_HORIZONTAL_BBOX_HEIGHT;
	}

}

void BigSophiaBullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive || state==BULLET_STATE_HIT_BRICK)
		return;
#pragma region set dam and direction
	
	if (timeDelayed >= timeDelayedMax)
	{
		isActive = false;
		timeDelayed = 0;
		return;
	}
	else if (state == BULLET_STATE_FLYING)
	{
		DebugOut(L"time dan big %d", timeDelayed);
		timeDelayed += dt;
		switch (typeBullet)
		{
		case BIG_SOPHIA_BULLET_LV1:
		{
			vx = BULLET_SPEED * nx;
			vy = BULLET_SPEED * ny;
			Entity::Update(dt);
			break;
		}
		case BIG_SOPHIA_BULLET_LV2:
		{

			if (ny == 0)
				angle += BULLET_LV2_ALPHA * nx * dt*1.5f;
			else 
				angle += BULLET_LV2_ALPHA * ny * dt*1.5f;
			radius += 0.3;
			if (nx == 0)
			{
				dx = startingPoint.first + (BULLET_LV2_RADIUS_SPACE + radius) * cos(angle) - x - 5;
				dy = startingPoint.second + (BULLET_LV2_RADIUS_SPACE + radius) * sin(angle) - y - 6;
			}
			else
			{
				dx = startingPoint.first + (BULLET_LV2_RADIUS_SPACE + radius) * cos(angle) - x - 3;
				dy = startingPoint.second + (BULLET_LV2_RADIUS_SPACE + radius) * sin(angle) - y;
			}
			break;
		}
		case BIG_SOPHIA_BULLET_LV3:
		{
			angle += BULLET_LV3_DELTA_DEGREE_PER_FRAME;
			if (nx != 0)
			{
				vx = BULLET_SPEED * nx;
				vy = BULLET_LV3_SPEED * sin(angle / 180);
			}
			if (ny != 0)
			{
				vx = BULLET_LV3_SPEED * sin(angle / 180);
				vy = BULLET_SPEED * ny;
			}
			Entity::Update(dt);
			break;
		}
		}
		HandlePlayerBulletCollision(coObjects);
	}
}

void BigSophiaBullet::Render()
{
	if (!isActive)
		return;
	int ani;
	if (state == BULLET_STATE_FLYING)
	{
		switch (typeBullet)
		{
		case BIG_SOPHIA_BULLET_LV1:
		{
			if (nx != 0)
			{
				ani = BULLET_BIG_SOPHIA_LV1_ANI_RIGHT;
				animationSet->at(ani)->Render(nx, x, y, alpha);
			}
			else if (ny!=0)
			{
				ani = BULLET_BIG_SOPHIA_LV1_ANI_TOP;
				animationSet->at(ani)->RenderTopBottom(ny, x, y, alpha);
			}
			break;
		}
		case BIG_SOPHIA_BULLET_LV2:
		{
			ani = BULLET_BIG_SOPHIA_LV2_ANI;

			if (x>startingPoint.first)
			{
				animationSet->at(ani)->Render(1, x, y, alpha);
			}
			else 
			{
				animationSet->at(ani)->Render(-1, x, y, alpha);
			}
			break;
		}
		case BIG_SOPHIA_BULLET_LV3:
		{
			if (nx != 0)
			{
				ani = BULLET_BIG_SOPHIA_LV3_ANI_RIGHT;
				animationSet->at(ani)->Render(nx, x, y, alpha);
			}
			else if (ny != 0)
			{
				ani = BULLET_BIG_SOPHIA_LV3_ANI_TOP;
				animationSet->at(ani)->RenderTopBottom(ny, x, y, alpha);
			}
			break;
		}
		default:
			break;
		}
	}
	else if (state == BULLET_STATE_HIT_BRICK)
	{
		ani = BULLET_BIG_SOPHIA_BANG;
		/*switch (typeBullet)
		{
		case BIG_SOPHIA_BULLET_LV3:
			animationSet->at(ani)->Render(1, xPosBLow, yPosBlow);
			break;
		default:*/
			animationSet->at(ani)->Render(1, x, y);
			if (animationSet->at(ani)->GetFrame() == animationSet->at(ani)->GetLastFrameIndex())
			{
				isActive = false;
				DebugOut(L"xoa dan big\n");
			}
		//	break;
		//}
	}
	RenderBoundingBox();
}

void BigSophiaBullet::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case BULLET_STATE_FLYING:
	{
		isHitBrick = false;
		isHitEnemy = false;
		isActive = true;
		break;
	}
	case BULLET_STATE_HIT_BRICK:
	{
		vx = 0;
		vy = 0;
		isHitBrick = true;
	}
	case BULLET_STATE_ENEMY:
	{
		vx = 0;
		vy = 0;
		isHitEnemy = true;
		break;
	}
	}
}

#include "Domes.h"
#include "Brick.h"
#include <chrono>
#include <thread>

void Domes::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DOMES_BBOX_WIDTH;
	bottom = y + DOMES_BBOX_HEIGHT;
}

Domes::Domes(float x, float y, LPGAMEENTITY t)
{
	//SetState(DOMES_STATE_WALKING_TOP_BOTTOM_RIGHT);
	enemyType = DOMES;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	dam = 1;
	this->dgravity = 3;
	nx = 1;
	this->target = t;
	health = DOMES_MAXHEALTH;
	isActive = false;
	directionclock = -1;
	isDamaged = false;
	bbARGB = 0;
	firstFollow = true;
	actived = false;
	aboveTarget = false;
}

void Domes::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);

#pragma region Xử lý Attack
	StartAttack();
#pragma endregion

#pragma region Xử lý State
	swapgravity();
#pragma endregion

#pragma region Xử lý tiền va chạm
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;

	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			bricks.push_back(coObjects->at(i));

	// turn off collision when die 
	if (state != DOMES_STATE_DIE)
		CalcPotentialCollisions(&bricks, coEvents);
#pragma endregion

#pragma region Xử lý logic khi va chạm
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

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

#pragma region Xử lý AI bam tuong va Acttack
		AIClimdWall(coEventsResult, nx, ny);
#pragma endregion

#pragma region Xử lý Active
		Activation();
#pragma endregion

	}
#pragma endregion
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Domes::Render()
{
	int ani;

	switch (dgravity)
	{
	case 1:

		if (isDamaged)
		{
			ani = DOMES_ANI_ATTACK_TOP_BOTTOM;
			animationSet->at(ani)->RenderTopBottom(1, x, y);
		}
		else if (vx > 0)
		{
			ani = DOMES_ANI_WALKING_TOP_BOTTOM_LEFT;
			animationSet->at(ani)->RenderTopBottom(1, x, y);
		}
		else
		{
			ani = DOMES_ANI_WALKING_TOP_BOTTOM_RIGHT;
			animationSet->at(ani)->RenderTopBottom(1, x, y);
		}
		break;

	case 2:
		if (isDamaged)
		{
			if (aboveTarget)
			{
				ani = DOMES_ANI_ATTACK_TOP_BOTTOM;
				animationSet->at(ani)->RenderTopBottom(1, x, y);
			}
			else {
				ani = DOMES_ANI_ATTACK_LEFT_RIGHT;
				animationSet->at(ani)->Render(1, x, y);
			}
		}
		else if (vy > 0)
		{
			ani = DOMES_ANI_WALKING_LEFT_RIGHT_BOTTOM;
			animationSet->at(ani)->Render(1, x, y);
		}
		else if (vy < 0)
		{
			ani = DOMES_ANI_WALKING_LEFT_RIGHT_TOP;
			animationSet->at(ani)->Render(1, x, y);
		}
		break;

	case 3:
		if (isDamaged)
		{
			ani = DOMES_ANI_ATTACK_TOP_BOTTOM;
			animationSet->at(ani)->RenderTopBottom(-1, x, y);
		}
		else if (vx > 0)
		{
			ani = DOMES_ANI_WALKING_TOP_BOTTOM_LEFT;
			animationSet->at(ani)->RenderTopBottom(-1, x, y);
		}
		else
		{
			ani = DOMES_ANI_WALKING_TOP_BOTTOM_RIGHT;
			animationSet->at(ani)->RenderTopBottom(-1, x, y);
		}
		break;

	case 4:
		if (isDamaged)
		{
			ani = DOMES_ANI_ATTACK_LEFT_RIGHT;
			animationSet->at(ani)->Render(-1, x, y);
		}
		else if (vy > 0)
		{
			ani = DOMES_ANI_WALKING_LEFT_RIGHT_BOTTOM;
			animationSet->at(ani)->Render(-1, x, y);
		}
		else if (vy < 0)
		{
			ani = DOMES_ANI_WALKING_LEFT_RIGHT_TOP;
			animationSet->at(ani)->Render(-1, x, y);
		}
		break;

	default:
		break;
	}


	RenderBoundingBox();
}

void Domes::AIClimdWall(vector<LPCOLLISIONEVENT> coEventsResult, float nx, float ny)
{
	switch (dgravity)
	{
	case 3:

		if (nx == 1)
		{
			if (vx < 0)
			{
				this->dgravity = 4;
				vx = 0; 
			}

		}
		else if (nx == -1)
		{
			if (vx > 0)
			{
				this->dgravity = 2;
				vx = 0;
				ny = 0;
			}
		}
		else if (nx == 0 && ny == -1)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (dynamic_cast<Brick*>(e->obj))
				{
					Brick* brick = dynamic_cast<Brick*>(e->obj);

					if (x + DOMES_BBOX_WIDTH > (brick->frameW + brick->x + DOMES_BBOX_WIDTH - 0.4f))
					{
						this->dgravity = 4;
						vy = 0;
					}

				}
			}

		}
		else if (ny == 1)
		{
			dgravity = 1;
			directionclock = random_directionclock();
			isDamaged = false;
		}
		break;
	case 4:
		if (ny == 1 && vx < 0)
		{
			this->dgravity = 1;

		}
		else if (ny == -1)
		{
			this->dgravity = 3;
		}
		else if (ny == 0 && nx != -1)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (dynamic_cast<Brick*>(e->obj))
				{
					Brick* brick = dynamic_cast<Brick*>(e->obj);

					if (y > (brick->frameH + brick->y - 0.5f))
					{
						this->dgravity = 1;
						nx = 0;

					}

				}
			}
		}
		else if (nx == -1)
		{
			dgravity = 2;
			directionclock = random_directionclock();
			isDamaged = false;
		}

		break;

	case 1:
		if (nx == 1 && vx < 0 && vy < 0)
		{
			this->dgravity = 4;
			vx = 0;

		}
		else if (nx == -1 && vx > 0 && vy < 0)
		{
			this->dgravity = 2;
			vx = 0;
		}
		else if (nx == 0 && ny == 1)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (dynamic_cast<Brick*>(e->obj))
				{
					Brick* brick = dynamic_cast<Brick*>(e->obj);

					if ((x + DOMES_BBOX_WIDTH > (brick->frameW + brick->x + DOMES_BBOX_WIDTH - 0.4f)) && (vx > 0))
					{
						this->dgravity = 4;
						vy = 0;

					}
					else if (x + DOMES_BBOX_WIDTH < brick->x && vx < 0)
					{
						this->dgravity = 2;
						vy = 0;
					}
				}
			}
		}
		else if (ny == -1)
		{
			dgravity = 3;
			directionclock = random_directionclock();
			isDamaged = false;
		}


		break;

	case 2:
		if (vy > 0 && nx == -1 && ny == 0)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (dynamic_cast<Brick*>(e->obj))
				{
					Brick* brick = dynamic_cast<Brick*>(e->obj);

					if (y + 0.5f > (brick->frameH + brick->y))
					{
						this->dgravity = 1;
						nx = 0;

					}
				}
			}
		}
		else if (vy < 0 && ny == 1)
		{
			this->dgravity = 1;
			vy = 0;
		}
		else if (nx == 1)
		{

			dgravity = 4;
			directionclock = random_directionclock();
			isDamaged = false;
		}
		else if (ny == -1 && vx > 0)
		{
			this->dgravity = 3;
			vy = 0;
		}
		break;
	default:

		break;
	}
}

void Domes::StartAttack()
{
	if (firstFollow)
	{
		delayAttack->Start();
		switch (dgravity)
		{
		case 1:
			if (target->x <= x + DOMES_BBOX_WIDTH && target->x >= x && target->y > y)
			{
				startAttack->Start();
				isDamaged = true;
				firstFollow = false;
			}
			break;
		case 2:
			if (target->y <= y + DOMES_BBOX_HEIGHT && target->y >= y && target->x < x)
			{
				startAttack->Start();
				isDamaged = true;
				firstFollow = false;
			}
			else if (target->x <= x + DOMES_BBOX_WIDTH && target->x >= x && target->y > y)
			{
				startAttack->Start();
				aboveTarget = true;
				isDamaged = true;
				firstFollow = false;
			}
			break;
		case 3:
			if (target->x <= x + DOMES_BBOX_WIDTH && target->x >= x && target->y < y)
			{
				startAttack->Start();
				isDamaged = true;
				firstFollow = false;
			}
			break;
		case 4:
			if (target->y <= y + DOMES_BBOX_HEIGHT && target->y >= y && target->x > x)
			{
				startAttack->Start();
				isDamaged = true;
				firstFollow = false;
			}
			break;


		default:
			break;
		}
	}
	else if (!firstFollow)
	{
		if (delayAttack->IsTimeUp())
		{
			switch (dgravity)
			{
			case 1:
				if (target->x <= x + DOMES_BBOX_WIDTH && target->x >= x && target->y > y)
				{
					startAttack->Start();
					isDamaged = true;
					delayAttack->Reset();
					delayAttack->Start();
				}
				break;
			case 2:
				if (target->y <= y + DOMES_BBOX_HEIGHT && target->y >= y && target->x < x)
				{
					startAttack->Start();
					isDamaged = true;
					delayAttack->Reset();
					delayAttack->Start();
				}
				else if (target->x <= x + DOMES_BBOX_WIDTH && target->x >= x && target->y > y)
				{
					startAttack->Start();
					aboveTarget = true;
					isDamaged = true;
					delayAttack->Reset();
					delayAttack->Start();
				}
				break;
			case 3:
				if (target->x <= x + DOMES_BBOX_WIDTH && target->x >= x && target->y < y)
				{
					startAttack->Start();
					isDamaged = true;
					delayAttack->Reset();
					delayAttack->Start();
				}
				break;
			case 4:
				if (target->y <= y + DOMES_BBOX_HEIGHT && target->y >= y && target->x > x)
				{
					startAttack->Start();
					isDamaged = true;
					delayAttack->Reset();
					delayAttack->Start();
				}
				break;


			default:
				break;
			}
		}
	}
}

void Domes::swapgravity()
{
	switch (directionclock)
	{
	case 1:
		if (isDamaged == true)
		{
			if (dgravity == 1)
			{
				SetState(DOMES_STATE_ATTACK_TOP_BOTTOM);
				vx = 0;
			}
			else if (dgravity == 3)
			{
				SetState(DOMES_STATE_ATTACK_TOP_BOTTOM);
				vx = 0;
			}
			else if (dgravity == 2)
			{
				if (aboveTarget)
				{
					SetState(DOMES_STATE_ATTACK_TOP_BOTTOM);
					vx = 0;
				}
				else {
					SetState(DOMES_STATE_ATTACK_LEFT_RIGHT);
					vy = 0;
				}
			}
			else if (dgravity == 4)
			{
				SetState(DOMES_STATE_ATTACK_LEFT_RIGHT);
				vy = 0;
			}
		}
		else if (dgravity == 1)
		{
			vy -= DOMES_GRAVITY;
			SetState(DOMES_STATE_WALKING_TOP_BOTTOM_RIGHT);
		}
		else if (dgravity == 2)
		{
			vx += DOMES_GRAVITY;
			SetState(DOMES_STATE_WALKING_LEFT_RIGHT_BOTTOM);
		}
		else if (dgravity == 3)
		{
			vy += DOMES_GRAVITY;
			SetState(DOMES_STATE_WALKING_TOP_BOTTOM_LEFT);
		}
		else if (dgravity == 4)
		{
			vx -= DOMES_GRAVITY;
			SetState(DOMES_STATE_WALKING_LEFT_RIGHT_TOP);
		}
		break;

	case -1:
		if (isDamaged == true)
		{
			if (dgravity == 1)
			{
				SetState(DOMES_STATE_ATTACK_TOP_BOTTOM);
				vx = 0;
			}
			else if (dgravity == 3)
			{
				SetState(DOMES_STATE_ATTACK_TOP_BOTTOM);
				vx = 0;
			}
			else if (dgravity == 2)
			{
				if (aboveTarget)
				{
					SetState(DOMES_STATE_ATTACK_TOP_BOTTOM);
					vx = 0;
				}
				else {
					SetState(DOMES_STATE_ATTACK_LEFT_RIGHT);
					vy = 0;
				}
			}
			else if (dgravity == 4)
			{
				SetState(DOMES_STATE_ATTACK_LEFT_RIGHT);
				vy = 0;
			}
		}
		else if (dgravity == 1)
		{
			vy -= DOMES_GRAVITY;
			SetStatenoclock(DOMES_STATE_WALKING_TOP_BOTTOM_RIGHT);
		}
		else if (dgravity == 2)
		{
			vx += DOMES_GRAVITY;
			SetStatenoclock(DOMES_STATE_WALKING_LEFT_RIGHT_BOTTOM);
		}
		else if (dgravity == 3)
		{
			vy += DOMES_GRAVITY;
			SetStatenoclock(DOMES_STATE_WALKING_TOP_BOTTOM_LEFT);
		}
		else if (dgravity == 4)
		{
			vx -= DOMES_GRAVITY;
			SetStatenoclock(DOMES_STATE_WALKING_LEFT_RIGHT_TOP);
		}
		break;

	default:
		break;
	}
}

void Domes::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case DOMES_STATE_DIE:
		y += DOMES_BBOX_HEIGHT - DOMES_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case DOMES_STATE_ATTACK_LEFT_RIGHT:
	{
		if (dgravity == 4)
		{
			if (startAttack->IsTimeUp())
			{
				vx = DOMES_ATTACK_SPEED;
				startAttack->Reset();
			}

		}
		else if (dgravity == 2 && !aboveTarget)
		{
			if (startAttack->IsTimeUp())
			{
				DebugOut(L"########### %d\n", 70000);
				vx = -DOMES_ATTACK_SPEED;
				startAttack->Reset();
			}
		}
	}
		break;
	case DOMES_STATE_ATTACK_TOP_BOTTOM:
		if (dgravity == 1)
		{
			if (startAttack->IsTimeUp())
			{
				vy = DOMES_ATTACK_SPEED;
				startAttack->Reset();
			}

		}
		else if (dgravity == 3)
		{
			if (startAttack->IsTimeUp())
			{
				vy = -DOMES_ATTACK_SPEED;
				startAttack->Reset();
			}
		}
		else if (dgravity == 2 && aboveTarget)
		{
			if (startAttack->IsTimeUp())
			{
				DebugOut(L"########### %d\n", 80000);
				vy = DOMES_ATTACK_SPEED;
				aboveTarget = false;
				startAttack->Reset();
			}
		}
		break;
	case DOMES_STATE_WALKING_TOP_BOTTOM_RIGHT:
		vx = DOMES_WALKING_SPEED;
		break;
	case DOMES_STATE_WALKING_TOP_BOTTOM_LEFT:
		vx = -DOMES_WALKING_SPEED;
		break;
	case DOMES_STATE_WALKING_LEFT_RIGHT_TOP:
		vy = -DOMES_WALKING_SPEED;
		break;
	case DOMES_STATE_WALKING_LEFT_RIGHT_BOTTOM:
		vy = DOMES_WALKING_SPEED;
		break;

	}
	//DebugOut(L"\nstartAttack:  %d", startAttack);
}

void Domes::SetStatenoclock(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case DOMES_STATE_DIE:
		y += DOMES_BBOX_HEIGHT - DOMES_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case DOMES_STATE_ATTACK_LEFT_RIGHT:
		if (dgravity == 4)
		{
			vx = DOMES_ATTACK_SPEED;
		}
		else if (dgravity == 2 && !aboveTarget)
		{
			vx = -DOMES_ATTACK_SPEED;
		}
		break;
	case DOMES_STATE_ATTACK_TOP_BOTTOM:
		if (dgravity == 1)
		{
			vy = DOMES_ATTACK_SPEED;

		}
		else if (dgravity == 3)
		{
			vy = -DOMES_ATTACK_SPEED;
		}
		else if (dgravity == 2)
		{
			vy = DOMES_ATTACK_SPEED;
		}
		break;
	case DOMES_STATE_WALKING_TOP_BOTTOM_RIGHT:
		vx = -DOMES_WALKING_SPEED;
		break;
	case DOMES_STATE_WALKING_TOP_BOTTOM_LEFT:
		vx = DOMES_WALKING_SPEED;
		break;
	case DOMES_STATE_WALKING_LEFT_RIGHT_TOP:
		vy = DOMES_WALKING_SPEED;
		break;
	case DOMES_STATE_WALKING_LEFT_RIGHT_BOTTOM:
		vy = -DOMES_WALKING_SPEED;
		break;

	}
}

void Domes::Activation()
{
	if (!isActive && !actived)
	{
		vx = 0;
	}
	else if (isActive && !actived)
	{
		vx = -DOMES_WALKING_SPEED;
		actived = true;
	}
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= DOMES_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}

int Domes::random_directionclock()
{
	srand(time(NULL));
	int res;
	for (int i = rand() % (5 - 1 + 1) + 1; i < 6; i++) {
		if (i % 2 == 0)
			return	res = 1;
		else return res = -1;
	}

}

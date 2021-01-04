#include "Domes.h"
#include <chrono>
#include <thread>

void Domes::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + DOMES_BBOX_WIDTH;
	bottom = y + DOMES_BBOX_HEIGHT;
}

Domes::Domes(float x, float y, LPGAMEENTITY t, int gravity)
{
	enemyType = EnemyType::DOMES;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	this->dam = 1;
	this->dgravity = gravity;
	nx = 1;
	this->target = t;
	health = DOMES_MAXHEALTH;
	isActive = false;
	isDamaged = false;
	firstFollow = true;
	actived = false;
	aboveTarget = false;
	isAttackingLeftRight = false;
	directionSet = 1;
}

void Domes::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	if (health <= 0)
	{
		this->SetState(DOMES_STATE_DIE);
		return;
	}
#pragma region Xử lý Attack
	StartAttack();
#pragma endregion

#pragma region Ngừng bay ngang
	//stopAttackLeftRight();
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
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK || coObjects->at(i)->GetType() == EntityType::TAG_SOFT_BRICK || coObjects->at(i)->GetType() == EntityType::TAG_GATE)
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

#pragma region Xử lý AI bam tuong va Attack
		AIClimdWall(coEventsResult, nx, ny);
#pragma endregion

	}
#pragma endregion
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

#pragma region Xử lý Active
	Activation();
#pragma endregion
}

void Domes::Render()
{
	int ani;

	if (state == DOMES_STATE_DIE)
	{
		ani = DOMES_ANI_DIE;
		if (animationSet->at(ani)->GetFrame() == animationSet->at(ani)->GetLastFrameIndex())
		{
			isDoneDeathAni = true;
			animationSet->at(ani)->ResetCurrentFrame();
		}
		animationSet->at(ani)->Render(nx, x, y - 2);
	}
	else {
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
				if (aboveTarget)
				{
					ani = DOMES_ANI_ATTACK_TOP_BOTTOM;
					animationSet->at(ani)->RenderTopBottom(1, x, y);
				}
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
	}
}

void Domes::AIClimdWall(vector<LPCOLLISIONEVENT> coEventsResult, float nx, float ny)
{
	switch (dgravity)
	{
	case 3:
	{
		if (nx == 1 && vx < 0)
		{
			this->dgravity = 4;
			vx = 0;
		}
		else if (nx == -1 && vx > 0)
		{
			this->dgravity = 2;
			vx = 0;
		}
		else if (nx == 0 && ny == -1)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (dynamic_cast<Brick*>(e->obj))
				{
					Brick* brick = dynamic_cast<Brick*>(e->obj);
					if (vx > 0)
					{
						if (x > (brick->GetBrickWidth() + brick->Getx()))
						{
							this->dgravity = 4;
							vy = 0;
						}
					}
					else if (vx < 0)
					{
						if (x + DOMES_BBOX_WIDTH < brick->Getx())
						{
							this->dgravity = 2;
							vy = 0;
						}
					}

				}
			}
		}
		else if (nx == 0 && ny == 1)
		{
			this->dgravity = 1;
			SetDirection();
			isDamaged = false;
		}
		break;
	}

	case 4:
	{
		if (ny == 1 && vy < 0)
		{
			this->dgravity = 1;

		}
		else if (ny == -1 && vy > 0)
		{
			this->dgravity = 3;
		}
		else if (ny == 0 && nx == 1)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (dynamic_cast<Brick*>(e->obj))
				{
					Brick* brick = dynamic_cast<Brick*>(e->obj);
					if (vy > 0)
					{
						if (y > (brick->GetBrickHeight() + brick->Gety()))
						{
							this->dgravity = 1;

						}
					}
					
					else if (vy < 0)
					{
						if (y + DOMES_BBOX_HEIGHT < brick->Gety())
						{
							this->dgravity = 3;
						}
					}

				}
			}
		}
		break;
	}

	case 1:
	{
		if (nx == 1 && vx < 0)
		{
			this->dgravity = 4;
			vx = 0;
		}
		else if (nx == -1 && vx > 0)
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

					if (vx > 0)
					{
						if (x + 0.5f > (brick->GetBrickWidth() + brick->Getx()))
						{
							this->dgravity = 4;
							vy = 0;
						}
					}
					else if (vx < 0)
					{
						if (x + DOMES_BBOX_WIDTH < brick->Getx())
						{
							this->dgravity = 2;
							vy = 0;
						}
					}
				}
			}
		}
		else if (nx == 0 && ny == -1)
		{
			this->dgravity = 3;
			SetDirection();
			isDamaged = false;
		}
		break;
	}

	case 2:
	{
		if (vy < 0 && ny == 1)
		{
			this->dgravity = 1;
			vy = 0;
		}
		else if (ny == -1 && vy > 0)
		{
			this->dgravity = 3;
			vy = 0;
		}
		else if (nx == -1 && ny == 0)
		{
			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (dynamic_cast<Brick*>(e->obj))
				{
					Brick* brick = dynamic_cast<Brick*>(e->obj);
					if (vy > 0)
					{
						if (y > (brick->GetBrickHeight() + brick->Gety()))
						{
							this->dgravity = 1;
							vx = 0;
						}
					}
					else if (vy < 0)
					{
						if (y + DOMES_BBOX_HEIGHT < brick->Gety())
						{
							this->dgravity = 3;
							vx = 0;
						}
					}
				}
			}
		}
		break;
	}
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
			if (target->Getx() <= x + DOMES_BBOX_WIDTH && target->Getx() >= x && target->Gety() > y)
			{
				startAttack->Start();
				isDamaged = true;
				firstFollow = false;
			}
			break;
		case 2:
			if (target->Getx() <= x + DOMES_BBOX_WIDTH && target->Getx() >= x && target->Gety() > y + 25.0f)
			{
				startAttack->Start();
				aboveTarget = true;
				isDamaged = true;
				firstFollow = false;
			}
			break;
		case 3:
			if (target->Getx() <= x + DOMES_BBOX_WIDTH && target->Getx() >= x && target->Gety() < y)
			{
				startAttack->Start();
				isDamaged = true;
				firstFollow = false;
			}
			break;
		case 4:
			if (target->Getx() >= x && target->Getx() <= x + DOMES_BBOX_WIDTH && target->Gety() > y + 25.0f)
			{
				startAttack->Start();
				aboveTarget = true;
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
				if (target->Getx() <= x + DOMES_BBOX_WIDTH && target->Getx() >= x && target->Gety() > y)
				{
					startAttack->Start();
					isDamaged = true;
					delayAttack->Reset();
					delayAttack->Start();
				}
				break;
			case 2:
				if (target->Getx() <= x + DOMES_BBOX_WIDTH && target->Getx() >= x && target->Gety() > y + 25.0f)
				{
					startAttack->Start();
					aboveTarget = true;
					isDamaged = true;
					delayAttack->Reset();
					delayAttack->Start();
				}
				break;
			case 3:
				if (target->Getx() <= x + DOMES_BBOX_WIDTH && target->Getx() >= x && target->Gety() < y)
				{
					startAttack->Start();
					isDamaged = true;
					delayAttack->Reset();
					delayAttack->Start();
				}
				break;
			case 4:
				if (target->Getx() >= x && target->Getx() <= x + DOMES_BBOX_WIDTH && target->Gety() > y + 25.0f)
				{
					startAttack->Start();
					aboveTarget = true;
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
		}
		else if (dgravity == 4)
		{
			if (aboveTarget)
			{
				SetState(DOMES_STATE_ATTACK_TOP_BOTTOM);
				vx = 0;
			}
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
}

void Domes::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case DOMES_STATE_DIE:
		isActive = false;
		vx = 0;
		vy = 0;
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
				vy = DOMES_ATTACK_SPEED;
				startAttack->Reset();
				this->dgravity = 1;
				aboveTarget = false;
			}
		}
		else if (dgravity == 4 && aboveTarget)
		{
			if (startAttack->IsTimeUp())
			{
				vy = DOMES_ATTACK_SPEED;
				startAttack->Reset();
				this->dgravity = 1;
				aboveTarget = false;
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
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->Getx(), target->Gety())) <= DOMES_SITEACTIVE_PLAYER)
	{
		isActive = true;
	}
}

void Domes::SetDirection()
{
	if (target->Getx() >= this->x)
	{
		this->nx = 1;
	}
	else this->nx = -1;
}

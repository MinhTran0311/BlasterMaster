#include "Mines.h"
#include "InjuringBrick.h"
#include "global.h"
#include "Sound.h"

void Mines::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MINES_BBOX_WIDTH;
	bottom = y + MINES_BBOX_HEIGHT;
}

void Mines::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt);
	if (health <= 0)
	{
		this->SetState(MINES_STATE_DIE);
		return;
	}
#pragma region fall down
	vy += MINES_GRAVITY * dt;
#pragma endregion
#pragma region Pre-collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();

	coEvents.clear();
	colliable_Objects->clear();

	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			colliable_Objects->push_back(coObjects->at(i));
	}
	//colliable_Objects->push_back(target);
	if (target->IsCollidingObject(this))
	{
		if (canExplosiving)
		{
			if (!isHitJason)
			{
				this->SetState(MINES_STATE_DIE);
				isHitJason = true;
			}
			canExplosiving = false;
		}
	}
#pragma endregion
	
#pragma region coillision
	CalcPotentialCollisions(colliable_Objects, coEvents);
	if (canExplosiving)
	{
		if (coEvents.size() == 0)
		{
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
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
			}
		}
	}

	//clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
#pragma region Active
	if (!isActive) return;
	else SetState(MINES_STATE_IDLE);
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->x, target->y)) <= MINES_SITEACTIVE_PLAYER)
	{
		Sound::GetInstance()->Play("MineBip", 0, 1);
		isActive = true;
	}
	else isActive = false;
#pragma endregion

}

void Mines::Render()
{
	//RenderBoundingBox();
	//if (vx > 0)
	//	nx = 1;
	//else
	//	nx = -1;

	int ani = MINES_ANI_IDLING;
	if (state == MINES_STATE_DIE) {
		ani = MINES_ANI_DIE;
		//DebugOut(L"So sprite %d\n", animationSet->at(ani)->GetFrame());
		if (animationSet->at(ani)->GetFrame() == 3)
		{
			animationSet->at(ani)->ResetCurrentFrame();
			isDoneDeath = true;
		}
		animationSet->at(ani)->Render(nx, x, y + MINES_BBOX_HEIGHT - MINES_BBOX_HEIGHT_DIE - 3);
	}
	else if (state == MINES_STATE_IDLE)
		animationSet->at(ani)->Render(nx, x, y);
}

Mines::Mines(float x, float y, LPGAMEENTITY t)
{
	SetState(MINES_STATE_IDLE);
	enemyType = MINES;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	nx = -1;
	this->target = t;
	health = MINES_MAXHEALTH;
	isActive = false;
	bbARGB = 250;
	isActive = false;
	canExplosiving = true;
	died = false;
}

void Mines::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
		case MINES_STATE_DIE:
		{
			int numberBullet = rand() % (6 - 4) + 4;
			if (!died)
			{
				died = true;
				for (int i = 0; i < numberBullet; i++)
				{
					Bullet* bullet = new SmallNavigatedEnemyBullet(x + MINES_BBOX_WIDTH / 2, y + MINES_BBOX_HEIGHT / 2, MINES, 1, -1, target);
					CGrid::GetInstance()->InsertGrid(bullet);
				}
			}
			isActive = false;
			break;
		}
		case MINES_ANI_IDLING:
		{
			break;
		}
	}
}

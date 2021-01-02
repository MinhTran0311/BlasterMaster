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
	//vy += MINES_GRAVITY * dt;
#pragma endregion
#pragma region Pre-collision
	/*for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			colliable_Objects.push_back(coObjects->at(i));
		if (state != MINES_STATE_DIE)
			CalcPotentialCollisions(&colliable_Objects, coEvents);
	}*/
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

#pragma region Active
	/*if (!isActive) return;
	else SetState(MINES_STATE_IDLE);*/
	if (GetDistance(D3DXVECTOR2(this->x, this->y), D3DXVECTOR2(target->Getx(), target->Gety())) <= MINES_SITEACTIVE_PLAYER)
	{
		Sound::GetInstance()->Play("MineBip", 1, 10000);
		//isActive = true;
	}
	else
	{
		Sound::GetInstance()->Stop("MineBip");
		//isActive = false;
	}
#pragma endregion

}

void Mines::Render()
{
	//RenderBoundingBox();
	//if (vx > 0)
	//	nx = 1;
	//else
	//	nx = -1;

	int ani;
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
	{
		ani = MINES_ANI_IDLING;
	}
	animationSet->at(ani)->Render(nx, x, y);
}

Mines::Mines(float x, float y, LPGAMEENTITY t)
{
	this->SetState(MINES_STATE_IDLE);
	enemyType = MINES;
	tag = EntityType::ENEMY;
	this->x = x;
	this->y = y;
	nx = -1;
	this->target = t;
	health = MINES_MAXHEALTH;
	bbARGB = 250;
	//isActive = false;
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
					LPBULLET bullet = new SmallNavigatedEnemyBullet(x + MINES_BBOX_WIDTH / 2, y + MINES_BBOX_HEIGHT / 2, MINES, 1, -1, target);
					CGrid::GetInstance()->InsertGrid(bullet);
				}
			}
			isDoneDeath = true;
			//isActive = false;
			break;
		}
		case MINES_ANI_IDLING:
		{
			break;
		}
	}
}

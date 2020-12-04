#include "SoftBrick.h"

void SoftBrick::Render()
{
	/*if (!isActive)
		return;*/
	int ani = SOFTBRICK_ANI_IDLE;

	if (state == SOFTBRICK_STATE_EXPLOSION)
	{
		ani = SOFTBRICK_ANI_EXPLOSION;
		DebugOut(L"Soft brick exp");
		if (animationSet->at(ani)->GetFrame() == 5)
		{
			animationSet->at(ani)->ResetCurrentFrame();
			isActive = false;
		}
		animationSet->at(ani)->Render(1, x - abs(EXPLOSION_BBOX_WIDTH - SOFT_BRICK_BBOX_WIDTH)/2, y);
	}
	else
	{
		animationSet->at(ani)->Render(1, x, y);
	}
	//RenderBoundingBox();
}

void SoftBrick::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	DebugOut(L"healt: %d\n", health);
	Entity::Update(dt);
	if (health <= 0)
	{
		this->SetState(SOFTBRICK_STATE_EXPLOSION);
		return;
	}

}

void SoftBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SOFT_BRICK_BBOX_WIDTH;
	b = y + SOFT_BRICK_BBOX_HEIGHT;
}

void SoftBrick::GetCenterBoundingBox(float& xPos, float& yPos)
{
	xPos = this->x + SOFT_BRICK_BBOX_WIDTH / 2;
	yPos = this->y + SOFT_BRICK_BBOX_HEIGHT / 2;
}

void SoftBrick::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case SOFTBRICK_STATE_EXPLOSION:

	default:
		break;
	}
}

SoftBrick::SoftBrick(int x, int y)
{
	isActive = true;
	this->SetPosition(x, y);
	frameH = SOFT_BRICK_BBOX_HEIGHT;
	frameW = SOFT_BRICK_BBOX_WIDTH;
	tag = EntityType::TAG_SOFT_BRICK;
	this->SetState(SOFTBRICK_STATE_IDLE);
	health = 1;
}

SoftBrick::~SoftBrick()
{
}

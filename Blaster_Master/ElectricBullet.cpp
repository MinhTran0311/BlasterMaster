#include "ElectricBullet.h"

ElectricBullet::ElectricBullet(float xPos, float yPos)
{
	SetState(ELECTRIC_BULLET_JASON_STATE_FIRE);
	DebugOut(L"Electric bullet\n");
	tag = EntityType::BULLET;
	typeBullet = EntityType::JASON_ELECTRIC_BULLET;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_JASON_ELECTRIC_BULLET));
	x = xPos;
	y = yPos + JASON_BBOX_HEIGHT / 2;
	dam = 1;
	timeDelayed = 0;
	timeDelayedMax = ELECTRIC_BULLET_JASON_DELAY;
}

ElectricBullet::~ElectricBullet()
{
}

void ElectricBullet::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case ELECTRIC_BULLET_JASON_STATE_FIRE:
	{
		isActive = true;
		break;
	}
	case ELECTRIC_BULLET_JASON_STATE_DONE:
	{
		isActive = false;
		break;
	}
	default:
		break;
	}
}

void ElectricBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ELECTRIC_BULLET_JASON_BBOX_WIDTH;
	bottom = y + ELECTRIC_BULLET_JASON_BBOX_HEIGHT;
}

void ElectricBullet::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive)
		return;
	timeDelayed += dt;
	Entity::Update(dt);
	if (timeDelayed >= timeDelayedMax)
		SetState(ELECTRIC_BULLET_JASON_STATE_DONE);
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == ENEMY)
		{
			//colliable_Objects->push_back(coObjects->at(i));
			if (this->IsCollidingObject(coObjects->at(i)))
			{
				coObjects->at(i)->AddHealth(-dam);
				DebugOut(L"xxxxxxxxxxxxxxxx %d", coObjects->at(i)->GetHealth());
			}
		}
	}

}

void ElectricBullet::Render()
{
	if (state == ELECTRIC_BULLET_JASON_STATE_DONE)
		return;
	else
		animationSet->at(ELECTRIC_BULLET_ANI)->Render(1, x, y);
}

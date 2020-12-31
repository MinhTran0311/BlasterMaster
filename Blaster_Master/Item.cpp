#include "Item.h"

Item::Item()
{
	tag = EntityType::ITEM;
	//lifetime = 0;
	this->SetState(ITEM_STATE_ACTIVE);
}

Item::~Item() {}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_BBOX_WIDTH;
	bottom = y + ITEM_BBOX_HEIGHT;
}

void Item::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive)
	{
		SetState(ITEM_ACTIVE_UNACTIVE);
		return;
	}
	//lifetime += dt;
	//if (lifetime > lifetimelimit)
	if(displayTimer->IsTimeUp())
	{
		SetState(ITEM_ACTIVE_UNACTIVE);
		return;
	}
	Entity::Update(dt);
}

void Item::Render()
{
	if (state == ITEM_ACTIVE_UNACTIVE)
		return;
	else
	{
		animationSet->at(0)->OldRender(x, y);
		//RenderBoundingBox();
	}
}

void Item::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case ITEM_STATE_ACTIVE:
		isActive = true;
		break;
	case ITEM_ACTIVE_UNACTIVE:
		isActive = false;
		break;
	default:
		break;
	}
}

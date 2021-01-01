#include "Item.h"

Item::Item(float xPos, float yPos, EntityType type, bool spawed)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_ITEM));
	tag = EntityType::ITEM;
	itemType = type;
	x = xPos;
	y = yPos;
	SetState(ITEM_STATE_ACTIVE);
	if (spawed)
	{
		isStatic = false;
		displayTimer = new Timer(ITEM_LIFETIME);
		displayTimer->Start();
	}
	DebugOut(L"tạo item\n");
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
	if(displayTimer->IsTimeUp() && isStatic)
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
		DebugOut(L"render item\n");
		int ani = -1;
		switch (itemType)
		{
		case TAG_ITEM_SINGLE_POWER_UP:
			ani = ITEM_ANI_POWER_UP;
			animationSet->at(ani)->Render(1, x, y);
			break;
		case TAG_ITEM_HOVER:
			ani = ITEM_ANI_HOVER;
			break;
		case TAG_ITEM_ELECTRIC:
			ani = ITEM_ANI_ELECTRIC;
			break;
		case TAG_ITEM_ROCKET:
			ani = ITEM_ANI_ROCKET;
			break;
		case TAG_ITEM_HOMINGMISSLES:
			ani = ITEM_ANI_HOMINGMISSLES;
			break;
		case TAG_ITEM_SINGLE_GUN_UP:
			ani = ITEM_ANI_SINGLE_GUN_UP;
			animationSet->at(ani)->Render(1, x, y);
			break;
		case TAG_ITEM_FULL_GUN_UP:
			ani = ITEM_ANI_FULL_GUN_UP;
			break;
		case TAG_ITEM_FULL_POWER_UP:
			ani = ITEM_ANI_FULL_POWER_UP;
			break;		
		case TAG_ITEM_HYBER_BEAM:
			ani = ITEM_ANI_HYBER_BEAM;
			break;		
		case TAG_ITEM_CRUSHER_BEAM:
			ani = ITEM_ANI_CRUSHER_BEAM;
			break;
		default:
			break;
		}
		animationSet->at(ani)->Render(1,x, y);
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

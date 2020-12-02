#pragma once
#include "Entity.h"
#include "Timer.h"

#define ITEM_ACTIVE_UNACTIVE 0
#define ITEM_STATE_ACTIVE 100

#define ITEM_BBOX_WIDTH		16
#define ITEM_BBOX_HEIGHT	16
class Item;
typedef Item* LPGAMEITEM;

class Item : public Entity
{
protected:
	EntityType typeItem;
	Timer* displayTimer;
	Timer* delayTimer;
	//DWORD lifetime;
	//DWORD lifetimelimit;
public:
	Item();
	~Item();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	EntityType getItemType() { return typeItem; };
	void setActive(bool _active) { isActive = _active; };
};
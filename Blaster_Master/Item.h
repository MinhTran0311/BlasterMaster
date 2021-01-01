#pragma once
#include "Entity.h"
#include "Timer.h"

#define ITEM_ACTIVE_UNACTIVE 0
#define ITEM_STATE_ACTIVE 100

#define ITEM_BBOX_WIDTH		15
#define ITEM_BBOX_HEIGHT	16

#define ITEM_ANI_POWER_UP		0
#define ITEM_ANI_HOVER			1
#define ITEM_ANI_ELECTRIC		2
#define ITEM_ANI_ROCKET			3
#define ITEM_ANI_HOMINGMISSLES	4
#define ITEM_ANI_SINGLE_GUN_UP	5
#define ITEM_ANI_FULL_GUN_UP	6
#define ITEM_ANI_FULL_POWER_UP	7
#define ITEM_ANI_HYBER_BEAM		8
#define ITEM_ANI_CRUSHER_BEAM	9
#define ITEM_LIFETIME	1500
#define ITEM_POWER_UP_RESTORE	1
#define ITEM_GUN_UP_RESTORE		1
class Item;
typedef Item* LPGAMEITEM;

class Item : public Entity
{
protected:
	EntityType itemType;
	Timer* displayTimer;
	Timer* delayTimer;
	bool isStatic = true;;
	//DWORD lifetime;
	//DWORD lifetimelimit;
public:
	Item(float xPos, float yPos, EntityType type,bool spawed =false);
	~Item();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	EntityType GetItemType() { return itemType; };
};
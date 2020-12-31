#pragma once
#include "Entity.h"
#include "Jason.h"
#include "Bullet.h"

#define ELECTRIC_BULLET_ANI		0

#define ELECTRIC_BULLET_JASON_BBOX_WIDTH	15
#define ELECTRIC_BULLET_JASON_BBOX_HEIGHT	63

#define ELECTRIC_BULLET_JASON_STATE_FIRE	0
#define ELECTRIC_BULLET_JASON_STATE_DONE	100

#define ELECTRIC_BULLET_JASON_DELAY		2000

class ElectricBullet : public Bullet
{
public:
	ElectricBullet(float xPos, float yPos);
	~ElectricBullet();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
};

#pragma once
#include "Bullet.h"
#include "global.h"

#include <math.h>
#define BULLET_SKULL_GRAVITY							0.002f
#define BULLET_SKULL_FRICTIONAL							0.002f
#define ROLLING_SPEED									0.05f
//bbox cho dan 
#define BULLET_SKULL_BBOX_WIDTH							6
#define BULLET_SKULL_BBOX_HEIGHT						6

//state
#define BULLET_SKULL_STATE_ROLLING						100
#define BULLET_SKULL_STATE_EXPLOSION					200

//animation

#define BULLET_SKULL_ANI_FALL_DOWN_AND_ROLLING			0
#define BULLET_SKULL_ANI_EXPLOSION						1


#define BULLET_SKULL_DELAY								2000
//kieu dan


class SkullBullet : public Bullet
{
public:
	SkullBullet(float posX, float posY, int direct);
	~SkullBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
	virtual void SetState(int state);
};
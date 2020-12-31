 #pragma once
#include "Bullet.h"
#include "global.h"

#include <math.h>
#define BULLET_SKULL_GRAVITY							0.002f
#define BULLET_SKULL_FRICTIONAL							1.01f
#define BULLET_SKULL_ROLLING							0.06f
#define BULLET_SKULL_BOUNCE								0.25f
//bbox cho dan 
#define BULLET_SKULL_BBOX_WIDTH							10
#define BULLET_SKULL_BBOX_HEIGHT						10

//state
#define BULLET_SKULL_STATE_ROLLING						100
#define BULLET_SKULL_STATE_EXPLOSION					200

//animation

#define BULLET_SKULL_ANI_FALL_DOWN_AND_ROLLING			0
#define BULLET_SKULL_ANI_EXPLOSION						1


#define BULLET_SKULL_DELAY								3000
//kieu dan


class SkullBullet : public Bullet
{
private:
	LPGAMEENTITY target;

	bool rolling = false;
	bool bounce = false;
	float bounceIndex = 0;
public:
	SkullBullet(float posX, float posY, int direct, LPGAMEENTITY t);
	~SkullBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
	virtual void SetState(int state);
};
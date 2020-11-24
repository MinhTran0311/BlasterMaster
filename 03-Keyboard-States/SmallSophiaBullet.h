#pragma once
#include "Bullet.h"
#include "global.h"

#include <math.h>
//bbox cho dan binh thuong
#define BULLET_SMALL_SOPHIA_NORMAL_HORIZONTAL_BBOX_WIDTH		5
#define BULLET_SMALL_SOPHIA_NORMAL_HORIZONTAL_BBOX_HEIGHT		5
#define BULLET_SMALL_SOPHIA_NORMAL_VERTICAL_BBOX_WIDTH			5
#define BULLET_SMALL_SOPHIA_NORMAL_VERTICAL_BBOX_HEIGHT		5
//state
#define BULLET_SMALL_SOPHIA_STATE_FLYING						0
#define BULLET_SMALL_SOPHIA_STATE_HIT_BRICK					100
#define BULLET_SMALL_SOPHIA_STATE_HIT_ENEMY					200

//animation
#define BULLET_SMALL_SOPHIA_NORMAL_ANI_RIGHT					0
//#define BULLET_SMALL_SOPHIA_NORMAL_ANI_TOP						1
//#define BULLET_SMALL_SOPHIA_UPGRADE_ANI_RIGHT					2
//#define BULLET_SMALL_SOPHIA_UPGRADE_ANI_TOP					3
#define BULLET_BANG										1

#define BULLET_SMALL_SOPHIA_DELAY								300
//kieu dan

#define SMALL_SOPHIA_DISTANCE_BLOWING_UP	2

class SmallSophiaBullet : public Bullet
{
public:
	SmallSophiaBullet(float posX, float posY, int level, int direct);
	~SmallSophiaBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
	virtual void SetState(int state);
};
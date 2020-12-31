#pragma once
#include "Bullet.h"
#include "global.h"
#include "Timer.h"
#include <math.h>

//bbox cho dan binh thuong
#define BULLET_BIG_SOPHIA_HORIZONTAL_BBOX_WIDTH		11
#define BULLET_BIG_SOPHIA_HORIZONTAL_BBOX_HEIGHT	8
#define BULLET_BIG_SOPHIA_VERTICAL_BBOX_WIDTH		8
#define BULLET_BIG_SOPHIA_VERTICAL_BBOX_HEIGHT		11
////bbox cho dan mau
//#define BULLET_BIG_SOPHIA_LV2_HORIZONTAL_BBOX_WIDTH		7
//#define BULLET_BIG_SOPHIA_LV2_HORIZONTAL_BBOX_HEIGHT	5
//#define BULLET_BIG_SOPHIA_LV2_VERTICAL_BBOX_WIDTH		5
//#define BULLET_BIG_SOPHIA_LV2_VERTICAL_BBOX_HEIGHT		7

#define BULLET_BIG_SOPHIA_LV3_HORIZONTAL_BBOX_WIDTH		7
#define BULLET_BIG_SOPHIA_LV3_HORIZONTAL_BBOX_HEIGHT	5
#define BULLET_BIG_SOPHIA_LV3_VERTICAL_BBOX_WIDTH		5
#define BULLET_BIG_SOPHIA_LV3_VERTICAL_BBOX_HEIGHT		7

//animation
#define BULLET_BIG_SOPHIA_LV1_ANI_RIGHT					0
#define BULLET_BIG_SOPHIA_LV1_ANI_TOP					1
#define BULLET_BIG_SOPHIA_LV2_ANI						0
#define BULLET_BIG_SOPHIA_LV3_ANI_RIGHT					2
#define BULLET_BIG_SOPHIA_LV3_ANI_TOP					3
#define BULLET_BIG_SOPHIA_BANG							4

#define BULLET_BIG_SOPHIA_DELAY							8000

#define BULLET_LV2_ALPHA						0.0025
#define BULLET_LV2_RADIUS_SPACE					5
#define BULLET_LV3_DELTA_DEGREE_PER_FRAME		60
#define BULLET_LV3_SPEED						0.6

class BigSophiaBullet : public Bullet
{
public:
	BigSophiaBullet(float posX, float posY, int level, int directionX=0, int directionY=0);
	~BigSophiaBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
private:
	double angle = 0;
	float radius = 0;

	pair<float, float> startingPoint;
};
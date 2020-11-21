#pragma once
#include "Bullet.h"
#include <math.h>
//bbox cho dan binh thuong
#define BULLET_JASON_NORMAL_HORIZONTAL_BBOX_WIDTH		23
#define BULLET_JASON_NORMAL_HORIZONTAL_BBOX_HEIGHT		5
#define BULLET_JASON_NORMAL_VERTICAL_BBOX_WIDTH			6
#define BULLET_JASON_NORMAL_VERTICAL_BBOX_HEIGHT		20
//bbox cho dan mau
#define BULLET_JASON_UPGRADE_HORIZONTAL_BBOX_WIDTH		24
#define BULLET_JASON_UPGRADE_HORIZONTAL_BBOX_HEIGHT		6
#define BULLET_JASON_UPGRADE_VERTICAL_BBOX_WIDTH		6
#define BULLET_JASON_UPGRADE_VERTICAL_BBOX_HEIGHT		24
//state
#define BULLET_JASON_STATE_FLYING								0
#define BULLET_JASON_STATE_HIT_BRICK					100
#define BULLET_JASON_STATE_HIT_ENEMY					200

//animation
#define BULLET_JASON_NORMAL_ANI_RIGHT					0
#define BULLET_JASON_NORMAL_ANI_TOP						1
#define BULLET_JASON_UPGRADE_ANI_RIGHT					2
#define BULLET_JASON_UPGRADE_ANI_TOP					3
#define BULLET_BANG										4

#define BULLET_JASON_DELAY								550
//kieu dan
#define JASON_NORMAL_BULLET								0
#define JASON_UPGRADE_BULLET							1

class JasonBullet : public Bullet
{
public:
	JasonBullet(float posX, float posY, int level, int direct, bool isGunFlip);
	~JasonBullet();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
	virtual void SetState(int state);
};
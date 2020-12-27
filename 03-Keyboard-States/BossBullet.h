#pragma once
#include "Bullet.h"
#include "global.h"
#include <math.h>

#define BOSS_ENEMY_BULLET_BBOX_WIDTH	8
#define BOSS_ENEMY_BULLET_BBOX_HEIGHT	8

#define BOSS_ENEMY_BULLET_STATE_FLYING		0
#define BOSS_ENEMY_BULLET_STATE_HIT_PLAYER	200

#define BOSS_ENEMY_BULLET_ANI			0
#define BOSS_BULLET_SPEED			0.012f
#define BOSS_ENEMY_DELAY			4000

class BossBullet : public Bullet
{
	LPGAMEENTITY target;
	float vt;
	float xBullet, yBullet, xTarget, yTarget;
	int isMoving;
public:
	BossBullet(float xPos, float yPos, LPGAMEENTITY t);
	~BossBullet();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
	float CalPositionTarget(LPGAMEENTITY target, float v);
	virtual void SetState(int state);
};

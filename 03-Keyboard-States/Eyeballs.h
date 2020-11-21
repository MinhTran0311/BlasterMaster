#pragma once
#include "Enemy.h"
#include "Jason.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>

#define EYEBALLS_WALKING_SPEED 0.05f;
#define EYEBALLS_GRAVITY 0.002f

#define EYEBALLS_BBOX_WIDTH 15
#define EYEBALLS_BBOX_HEIGHT 15
#define EYEBALLS_BBOX_HEIGHT_DIE 9

#define EYEBALLS_STATE_IDLE 0
#define EYEBALLS_STATE_FLYING 100
#define EYEBALLS_STATE_ATTACKING 200
#define EYEBALLS_STATE_DIE 300

#define EYEBALLS_ANI_FLYING 0
#define EYEBALLS_ANI_IDLE 1
#define EYEBALLS_ANI_DIE 2

#define EYEBALLS_SITEFOLLOW_PLAYER			50
#define EYEBALLS_SITEACTIVE_PLAYER			150

#define EYEBALLS_TIME_FLY 3000
#define EYEBALLS_TIME_ATTACK 2500

#define EYEBALLS_MAXHEALTH					1


class Eyeballs : public Enemy
{
	bool isFollow;
	LPGAMEENTITY target;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	Timer* flyTimer = new Timer(EYEBALLS_TIME_FLY);
	Timer* attackTimer = new Timer(EYEBALLS_TIME_ATTACK);
	virtual void Render();

public:
	Eyeballs(float x, float y, LPGAMEENTITY t);

	void FlyAndAttackTarget(LPGAMEENTITY target);
	virtual void SetState(int state);
	void SelfDestroy();
};
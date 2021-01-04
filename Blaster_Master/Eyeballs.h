#pragma once
#include "Enemy.h"
#include "Jason.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>
#include <cstdlib>
#include <cmath>
#include "BigNavigatedEnemyBullet.h"
#include "Bullet.h"

#define EYEBALLS_FLYING_SPEED 0.05f
#define EYEBALLS_GRAVITY 0.002f

#define EYEBALLS_BBOX_WIDTH 15
#define EYEBALLS_BBOX_HEIGHT 15
#define EYEBALLS_BBOX_HEIGHT_DIE 9

#define EYEBALLS_STATE_IDLE 0
#define EYEBALLS_STATE_FLYING 100
#define EYEBALLS_STATE_ATTACKING 200
#define EYEBALLS_STATE_DIE 300

#define EYEBALLS_ANI_FLYING 0
#define EYEBALLS_ANI_ATTACKING 1
#define EYEBALLS_ANI_IDLE 2
#define EYEBALLS_ANI_DIE 3

#define EYEBALLS_SITEACTIVE_PLAYER			170

#define EYEBALLS_TIME_FLY 1600
#define EYEBALLS_TIME_IDLE 900
#define EYEBALLS_TIME_ATTACK 1100

#define EYEBALLS_MAXHEALTH					3


class Eyeballs : public Enemy
{
private:
	Random* r = new Random();
	bool canFly;
	bool canAttack;
	bool canIdle;

	Timer* flyTimer = new Timer(EYEBALLS_TIME_FLY);
	Timer* idleTimer = new Timer(EYEBALLS_TIME_IDLE);
	Timer* attackTimer = new Timer(EYEBALLS_TIME_ATTACK);

public:
	Eyeballs(float x, float y, LPGAMEENTITY t);
	
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void SetState(int state);

	void FlyAndAttackTarget();
	virtual void shootBulletToTarget();
	virtual void setRandomVxVy(float& vx, float& vy);
};
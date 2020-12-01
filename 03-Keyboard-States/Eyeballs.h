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
#define EYEBALLS_ANI_IDLE 1
#define EYEBALLS_ANI_DIE 2

#define EYEBALLS_SITEFOLLOW_PLAYER			50
#define EYEBALLS_SITEACTIVE_PLAYER			150

#define EYEBALLS_TIME_FLY_OR_ATTACK 3000
#define EYEBALLS_TIME_IDLE 300

#define EYEBALLS_MAXHEALTH					1


class Eyeballs : public Enemy
{
	LPGAMEENTITY target;
	Random* r = new Random();
	bool canFly;
	bool canAttack;
	bool canIdle;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	Timer* flyOrAttackTimer = new Timer(EYEBALLS_TIME_FLY_OR_ATTACK);
	Timer* idleTimer = new Timer(EYEBALLS_TIME_IDLE);
	virtual void Render();
	virtual void setRandomVxVy(float& vx, float& vy);
public:
	Eyeballs(float x, float y, LPGAMEENTITY t);
	void FlyAndAttackTarget();
	virtual void SetState(int state);
	void SelfDestroy();
};
#pragma once
#include "Enemy.h"
#include "Jason.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>
#include "Timer.h"
#include "BigNavigatedEnemyBullet.h"
#include "Bullet.h"
#include <random>

#define TELEPORTERS_BBOX_WIDTH 23 
#define TELEPORTERS_BBOX_HEIGHT 31
#define TELEPORTERS_BBOX_HEIGHT_DIE 9

#define TELEPORTERS_STATE_IDLE 0
#define TELEPORTERS_STATE_TRANSFORMING 100
#define TELEPORTERS_STATE_ATTACKING 200
#define TELEPORTERS_STATE_DIE 300

#define TELEPORTERS_ANI_IDLE 0
#define TELEPORTERS_ANI_TRANSFORMING 1
#define TELEPORTERS_ANI_ATTACKING 2
#define TELEPORTERS_ANI_DIE 3

#define TELEPORTERS_SITEATTACK_PLAYER	300
#define TELEPORTERS_SITEACTIVE_PLAYER	100

#define TELEPORTERS_DELAY_ATTACK 700
#define TELEPORTERS_SHOOT_BULLET 800
#define TELEPORTERS_RELAX_ATTACK 2000
#define TELEPORTERS_DELAY_IDLE 700
#define TELEPORTERS_MAXHEALTH	1


class Teleporters : public Enemy
{
private:
	bool isAttack;
	bool finish_transformation;

	Timer* delayAttackTimer = new Timer(TELEPORTERS_DELAY_ATTACK);
	Timer* shootBulletTimer = new Timer(TELEPORTERS_SHOOT_BULLET);
	Timer* relaxAttackTimer = new Timer(TELEPORTERS_RELAX_ATTACK);
	Timer* delayIdleTimer = new Timer(TELEPORTERS_DELAY_IDLE);
	bool isTransporting;
	bool isAttacking;
	bool isShooting;
	bool re_tranform;
	bool isIdling;
	bool enoughTimeIdle;
	bool doneJump;
	int teleTimesAttack;
	int x_Max, x_Min, y_Max, y_Min;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void Render();
	virtual void shootBulletToTarget();

	Teleporters(float x, float y, LPGAMEENTITY t, int x_Tele_Min, int y_Tele_Min, int x_Tele_Max, int y_Tele_Max);
	Teleporters(float x, float y);
	void AttackTarget(LPGAMEENTITY target, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void SetState(int state);
	bool TestTele(float x_Pos, float y_Pos, vector<LPGAMEENTITY>* coObjects = NULL);
	void SelfDestroy();
};
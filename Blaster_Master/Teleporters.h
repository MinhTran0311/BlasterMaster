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

#define TELEPORTERS_DELAY_ATTACK 900
#define TELEPORTERS_SHOOT_BULLET 800
#define TELEPORTERS_RELAX_ATTACK 2000
#define TELEPORTERS_DELAY_IDLE	 700
#define TELEPORTERS_TELE		 100
#define TELEPORTERS_MAXHEALTH	4


class Teleporters : public Enemy
{
private:
	bool isAttack;
	bool finish_transformation;
	bool isTransporting;
	bool isAttacking;
	bool isShooting;
	bool re_tranform;
	bool isIdling;
	bool enoughTimeIdle;
	int teleTimesAttack;
	int x_Max, x_Min, y_Max, y_Min;
	float x_Pos_Old;
	float y_Pos_Old;
	float x_Pos_New;
	float y_Pos_New;
	bool firstTele;

	Timer* delayAttackTimer = new Timer(TELEPORTERS_DELAY_ATTACK);
	Timer* shootBulletTimer = new Timer(TELEPORTERS_SHOOT_BULLET);
	Timer* relaxAttackTimer = new Timer(TELEPORTERS_RELAX_ATTACK);
	Timer* delayIdleTimer = new Timer(TELEPORTERS_DELAY_IDLE);
	Timer* telePositionOldTimer = new Timer(TELEPORTERS_TELE);
public:
	Teleporters(float x, float y, LPGAMEENTITY t, int x_Tele_Min, int y_Tele_Min, int x_Tele_Max, int y_Tele_Max);
	Teleporters(float x, float y);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	
	void AttackTarget(LPGAMEENTITY target, vector<LPGAMEENTITY>* coObjects = NULL);
	bool TestTele(float x_Pos, float y_Pos, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void shootBulletToTarget();
};
#pragma once
#include "Enemy.h"
#include "Grid.h"
#include "Jason.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>
#include "Timer.h"
#include "BigNavigatedEnemyBullet.h"
#include "Bullet.h"

#define CANNONS_BBOX_WIDTH 25
#define CANNONS_BBOX_HEIGHT 25
#define CANNONS_BBOX_HEIGHT_DIE 9

#define CANNONS_STATE_IDLE 0
#define CANNONS_STATE_ATTACKING_LEFT 100
#define CANNONS_STATE_ATTACKING_TOP 200
#define CANNONS_STATE_DIE 300

#define CANNONS_ANI_IDLE 0
#define CANNONS_ANI_ATTACKING_LEFT 1
#define CANNONS_ANI_ATTACKING_TOP 2
#define CANNONS_ANI_DIE 3

#define CANNONS_SITEATTACK_PLAYER 100

#define CANNONS_DELAY_ATTACK 500

#define CANNONS_MAXHEALTH					1

#define CANNONS_MAX_BULLETS	4


class Cannons : public Enemy
{
	bool isAttack;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void Render();
	Timer* delayAttackTimer = new Timer(CANNONS_DELAY_ATTACK);
	bool changeAttackToTopBottom;
	bool isAttackTop;
	
public:
	Cannons(float x, float y, LPGAMEENTITY t);

	void AttackTarget(LPGAMEENTITY target);
	virtual void SetState(int state);
	void SelfDestroy();
};
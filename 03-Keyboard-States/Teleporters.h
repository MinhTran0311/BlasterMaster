#pragma once
#include "Enemy.h"
#include "Jason.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>

//#define TELEPORTERS_WALKING_SPEED 0.05f;
#define TELEPORTERS_GRAVITY		0.002f

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

#define TELEPORTERS_SITEATTACK_PLAYER			10
#define TELEPORTERS_SITEACTIVE_PLAYER			20

#define TELEPORTERS_MAXHEALTH					1


class Teleporters : public Enemy
{
	bool isAttack;
	LPGAMEENTITY target;
	bool finish_transformation = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void Render();

public:
	Teleporters(float x, float y, LPGAMEENTITY t);

	void AttackTarget(LPGAMEENTITY target);
	virtual void SetState(int state);
	void SelfDestroy();
};
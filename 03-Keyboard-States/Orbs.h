#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Timer.h"


#define MOVING_SPEED	0.05f


#define ORBS_STATE_FLY		100
#define ORBS_STATE_ATTACK	200
#define ORBS_STATE_FLIP	400
#define ORBS_STATE_DIE		300

#define ORBS_ANI_FLY		0
#define ORBS_ANI_FLIP	1
#define ORBS_ANI_DIE		2

#define FLYING_SPEED	0.05f;

#define BBOX_WIDTH 18
#define BBOX_HEIGHT 15
//#define BBOX_HEIGHT_DIE 9

#define MAXHEALTH	1
#define MIN_COOLDOWN 500
#define MAX_COOLDOWN 5000

#define TARGET_RANGE 200


class Orbs : public Enemy
{
	float collectionX;
	bool isflip = false;
	int time = 0;
	bool canAttack = false;
	bool isTargeting;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
	virtual bool inTargetRange();
public:
	Orbs(float x, float y, LPGAMEENTITY t);
	virtual void Attack(LPGAMEENTITY target = NULL);
	virtual void SetState(int state);
	void SelfDestroy();
};

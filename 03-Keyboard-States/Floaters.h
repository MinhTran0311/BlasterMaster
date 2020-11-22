#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>
#include <cstdlib>
#include <cmath>
#define MOVING_SPEED	0.05f


#define FLOATERS_STATE_FLY		100
#define FLOATERS_STATE_ATTACK	200
#define FLOATERS_STATE_DIE		300

#define FLOATERS_ANI_FLY		0
#define FLOATERS_ANI_ATTACK		1
#define FLOATERS_ANI_DIE		2

#define FLYING_SPEED	0.05f;

#define BBOX_WIDTH 18
#define BBOX_HEIGHT 15
//#define BBOX_HEIGHT_DIE 9

#define MAXHEALTH	1
#define MIN_COOLDOWN 500
#define MAX_COOLDOWN 5000

#define TARGET_RANGE 200

class Floaters : public Enemy
{
	bool canAttack = true;
	bool isTargeting;
	Random* r = new Random();
	Timer* cooldownTimer = new Timer(r->getRandomInt(500,5000));
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
	virtual bool inTargetRange();
	virtual void setRandomVxVy(float& vx, float& vy);
public:
	Floaters(float x, float y, LPGAMEENTITY t);
	virtual void Attack(LPGAMEENTITY target=NULL);
	virtual void SetState(int state);
	void SelfDestroy();
};

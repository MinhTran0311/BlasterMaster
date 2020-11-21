#pragma once
#include "Entity.h"
#include "Enemy.h"




#define MOVING_SPEED	0.05f


#define SKULLS_STATE_FLY		100
#define SKULLS_STATE_ATTACK	200
#define SKULLS_STATE_DIE		300

#define SKULLS_ANI_FLY		0
#define SKULLS_ANI_ATTACK		1
#define SKULLS_ANI_DIE		2

#define FLYING_SPEED	0.05f;

#define BBOX_WIDTH 18
#define BBOX_HEIGHT 18
//#define BBOX_HEIGHT_DIE 9 

#define MAXHEALTH	1
#define MIN_COOLDOWN 500
#define MAX_COOLDOWN 5000

#define TARGET_RANGE 200

class Skulls : public Enemy
{


	bool canAttack = true;
	bool isTargeting;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
	virtual bool inTargetRange();
public:
	Skulls(float x, float y, LPGAMEENTITY t);
	virtual void Attack(LPGAMEENTITY target = NULL);
	virtual void SetState(int state);
	void SelfDestroy();
};
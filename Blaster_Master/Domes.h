#pragma once
#include "Enemy.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>

#define DOMES_WALKING_SPEED						0.03f;
#define DOMES_GRAVITY							0.008f

#define DOMES_ATTACK_SPEED						0.15f

#define DOMES_BBOX_WIDTH						18
#define DOMES_BBOX_HEIGHT						18
#define DOMES_BBOX_HEIGHT_DIE					9

#define DOMES_STATE_WALKING_TOP_BOTTOM_RIGHT	100
#define DOMES_STATE_WALKING_TOP_BOTTOM_LEFT		200
#define DOMES_STATE_WALKING_LEFT_RIGHT_TOP		300
#define DOMES_STATE_WALKING_LEFT_RIGHT_BOTTOM	400
#define DOMES_STATE_ATTACK_LEFT_RIGHT			500
#define DOMES_STATE_ATTACK_TOP_BOTTOM			600
#define DOMES_STATE_DIE							700

#define DOMES_ANI_WALKING_TOP_BOTTOM_RIGHT		0
#define DOMES_ANI_WALKING_TOP_BOTTOM_LEFT		1
#define DOMES_ANI_WALKING_LEFT_RIGHT_TOP		2
#define DOMES_ANI_WALKING_LEFT_RIGHT_BOTTOM		3

#define DOMES_ANI_ATTACK_LEFT_RIGHT				4
#define DOMES_ANI_ATTACK_TOP_BOTTOM				5



#define DOMES_ANI_DIE							6

#define DOMES_SITEACTIVE_PLAYER					270

#define DOMES_TIME_ATTACK						700

#define DOMES_TIME_DELAYATTACK					1500

#define DOMES_MAXHEALTH							1

class Domes : public Enemy
{
private:
	int dgravity;
	int directionSet;
	bool firstFollow;
	bool actived;
	bool aboveTarget;
	bool isAttackingLeftRight;

	Timer* delayAttack = new Timer(DOMES_TIME_DELAYATTACK);
	Timer* startAttack = new Timer(DOMES_TIME_ATTACK);

public:
	Domes(float x, float y, LPGAMEENTITY t, int gravity);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

	virtual void AIClimdWall(vector<LPCOLLISIONEVENT> coEventsResult, float nx, float ny);
	virtual void StartAttack();
	virtual void swapgravity();
	virtual void SetState(int state);
	virtual void Activation();

	void SetDirection();
};


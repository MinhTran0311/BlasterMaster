#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Jason.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>


#define JUMPER_WALKING_SPEED 0.05f;
#define JUMPER_JUMP_SPEED_Y 0.4f;
#define JUMPER_GRAVITY		0.002f

#define JUMPER_BBOX_WIDTH 17
#define JUMPER_BBOX_HEIGHT 25
#define JUMPER_BBOX_HEIGHT_DIE 26

#define JUMPER_STATE_WALKING 100
#define JUMPER_STATE_JUMP 300
#define JUMPER_STATE_DIE 200

#define JUMPER_ANI_WALKING 0
#define JUMPER_ANI_DIE 1

#define JUMPER_SITEFOLLOW_PLAYER			50
#define JUMPER_SITEACTIVE_PLAYER			150

#define JUMPER_MAXHEALTH					1



class Jumpers : public Enemy
{
	bool isFollow;
	int indexFollow=0;
	LPGAMEENTITY target;
	bool isJump = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void Render();
	int randomTimeJump();
public:
	Jumpers(float x, float y, LPGAMEENTITY t);

	void FollowTarget(LPGAMEENTITY target);
	virtual void SetState(int state);
	void SelfDestroy();
};

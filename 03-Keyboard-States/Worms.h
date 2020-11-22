#pragma once
#include "Enemy.h"
#include "Jason.h"
#include "Timer.h"
#include "Brick.h"
#include <ctime>

#define WORM_WALKING_SPEED 0.05f;
<<<<<<< HEAD
#define WORM_GRAVITY		0.00002f
=======
#define WORM_GRAVITY		0.0002f
>>>>>>> main

#define WORM_BBOX_WIDTH 17
#define WORM_BBOX_HEIGHT 8
#define WORM_BBOX_HEIGHT_DIE 9

#define WORM_STATE_WALKING 100
#define WORM_STATE_DIE 200

#define WORM_ANI_WALKING 0
#define WORM_ANI_DIE 1

<<<<<<< HEAD
#define WORM_SITEFOLLOW_PLAYER			150
=======
#define WORM_SITEFOLLOW_PLAYER			100
>>>>>>> main
#define WORM_SITEACTIVE_PLAYER			150

#define WORM_MAXHEALTH					1


class Worm : public Enemy
{
	LPGAMEENTITY target;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects=NULL);
	virtual void Render();

public:
	Worm(float x, float y, LPGAMEENTITY t);

	void FollowTarget(LPGAMEENTITY target);
	virtual void SetState(int state);
	void SelfDestroy();
};
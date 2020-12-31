#pragma once
#include "Bullet.h"
#include "global.h"
#include <math.h>
#include <random>
#include <ctime>
#include <cstdlib>
#include "Timer.h"

#define SMALL_NAVI_ENEMY_BULLET_BBOX_WIDTH			7
#define SMALL_NAVI_ENEMY_BULLET_BBOX_HEIGHT			7

#define SMALL_NAVI_ENEMY_BULLET_STATE_HIT_BRICK		0
#define SMALL_NAVI_ENEMY_BULLET_STATE_HIT_PLAYER	100
#define SMALL_NAVI_ENEMY_BULLET_STATE_FLYING		200

#define SMALL_NAVI_ENEMY_BULLET_ANI					0
#define SMALL_NAVI_ENEMY_BULLET_BANG				1
#define BULLET_SPEED_OTHERS							0.15f
#define BULLET_SPEED_MINES							0.1f
#define SMALL_BULLET_ENEMY_DELAY					1000
#define BULLET_TIMER_UP								150

class SmallNavigatedEnemyBullet : public Bullet
{
	LPGAMEENTITY target;
	float vt;
	float xBullet, yBullet, xTarget, yTarget;
	bool isMoving;
	Timer* timeFlyUp = new Timer(BULLET_TIMER_UP);
public:
	SmallNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x, int direct_y, LPGAMEENTITY t);
	~SmallNavigatedEnemyBullet();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
	void CalVelocity(float&, float&, LPGAMEENTITY);
	float RandomVx();
	virtual void SetState(int state);
};

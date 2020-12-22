#pragma once
#include "Bullet.h"
#include "Cannons.h"
#include "global.h"
#include <math.h>

#define BIG_NAVI_ENEMY_BULLET_BBOX_WIDTH	7
#define BIG_NAVI_ENEMY_BULLET_BBOX_HEIGHT	7

#define BIG_NAVI_ENEMY_BULLET_STATE_HIT_BRICK	0
#define BIG_NAVI_ENEMY_BULLET_STATE_HIT_PLAYER	100

#define BIG_NAVI_ENEMY_BULLET_ANI			0
#define BIG_NAVI_ENEMY_BULLET_BANG			1
#define CANNONS_BULLET_SPEED			0.2f
#define BULLET_SPEED_OTHERS				0.15f
#define BIG_BULLET_ENEMY_DELAY			1000
class BigNavigatedEnemyBullet : public Bullet
{
	LPGAMEENTITY target;
	float vt;
	float xBullet, yBullet, xTarget, yTarget;
	int isMoving;
public:
	BigNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x, int direct_y, LPGAMEENTITY t);
	~BigNavigatedEnemyBullet();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
	float CalPositionTarget(LPGAMEENTITY target, float v);
	virtual void SetState(int state);
};

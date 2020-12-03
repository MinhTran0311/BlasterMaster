#pragma once
#include "Bullet.h"
#include "Cannons.h"
#define BIG_NAVI_ENEMY_BULLET_BBOX_WIDTH	7
#define BIG_NAVI_ENEMY_BULLET_BBOX_HEIGHT	7

#define BIG_NAVI_ENEMY_BULLET_ANI			0
#define ENEMY_BULLET_BANG					1
#define CANNONS_BULLET_SPEED			0.15f
#define BIG_BULLET_ENEMY_DELAY			700
class BigNavigatedEnemyBullet : public Bullet
{
	LPGAMEENTITY target;
	float vt;
	float xBullet, yBullet, xTarget, yTarget;
public:
	BigNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x, int direct_y, LPGAMEENTITY t);
	~BigNavigatedEnemyBullet();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void Render();
	float CalPositionTarget(LPGAMEENTITY target, float v);
};


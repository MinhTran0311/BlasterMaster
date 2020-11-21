#pragma once
#include "Bullet.h"
#include "Cannons.h"
#define BIG_NAVI_ENEMY_BULLET_BBOX_WIDTH	7
#define BIG_NAVI_ENEMY_BULLET_BBOX_HEIGHT	7

#define BIG_NAVI_ENEMY_BULLET_ANI			0
#define ENEMY_BULLET_BANG					1
#define CANNONS_BULLET_SPEED			0.25f
#define BIG_BULLET_ENEMY_DELAY			700
class BigNavigatedEnemyBullet : public Bullet
{
public:
	BigNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x, int direct_y);
	BigNavigatedEnemyBullet(float posX, float posY, int type_enemy, int direct_x, int direct_y, LPGAMEENTITY Target);
	~BigNavigatedEnemyBullet();
	int bullet_speed;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	void CalcDistanceEquation(float& equa_a, float& equa_b, float target_x, float target_y, float x, float y);
	void Render();
};


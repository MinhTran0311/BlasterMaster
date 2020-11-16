#pragma once
#include "Entity.h"

#define ENEMY_MOVING_SPEED		0.1f

#define ENEMY_STATE_IDLE 100
#define ENEMY_STATE_MOVING_LEFT 200
#define ENEMY_STATE_MOVING_RIGHT 300

#define ENEMY_ANI_MOVING 1
#define ENEMY_ANI_IDLE 2


class Enemy : public Entity
{
protected:
	//int HP;
	bool isDamaged;
	int DamageofEnemy;
	bool isActive;
	bool isArmor;
	bool isBoss;
	bool isDeath;
	int dam;
	//int health;
	LPGAMEENTITY target;
	EnemyType enemyType;

public:
	Enemy();
	~Enemy();
	virtual void Render() = 0;
	virtual bool IsContain(RECT rect1, RECT rect2);
	virtual float GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* objects = NULL) = 0;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
	bool GetDamage() {return dam;};
	//void DeleteTarger() { target = NULL; };

};
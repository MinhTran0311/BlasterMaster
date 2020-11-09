#pragma once
#pragma once
#pragma once
#include "GameObject.h"

#define ENEMY_JUMPING_SPEED		0.1f

#define ENEMY_STATE_IDLE 0
#define ENEMY_STATE_JUMPING_LEFT 100
#define ENEMY_STATE_JUMPING_RIGHT 200

#define ENEMY_ANI_JUMPING_LEFT 2
#define ENEMY_ANI_JUMPING_RIGHT 3
#define ENEMY_ANI_IDLE_LEFT 4
#define ENEMY_ANI_IDLE_RIGHT 5


class CEnemy : public CGameObject
{

};

class CEnemy1 : public CEnemy
{
public:
	CEnemy1(float x, float y);
	CEnemy1();
	void Update(DWORD dt);
	void Render();
};
class CEnemy2 : public CEnemy
{
public:
	CEnemy2(float x, float y);
	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};
class CEnemy3 : public CEnemy
{

};
#pragma once
#include "Entity.h"
#include "Enemy.h"
#define WORM_MOVING_SPEED		0.1f

#define ENEMY_STATE_IDLE 100
#define ENEMY_STATE_MOVING_LEFT 200
#define ENEMY_STATE_MOVING_RIGHT 300

#define ENEMY_ANI_MOVING 1
#define ENEMY_ANI_IDLE 2


class CSkulls : public Enemy
{
	CSkulls(float x, float y);
	CSkulls();
	void Update(DWORD dt);
	void Render();
};
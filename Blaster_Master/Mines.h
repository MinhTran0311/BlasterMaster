#pragma once
#pragma once
#include "Enemy.h"
#include "Jason.h"
#include "global.h"
#include "Timer.h"
#include "Brick.h"
#include "SmallNavigatedEnemyBullet.h"
#include <ctime>
#include "Player.h"
#include "Grid.h"

#define MINES_GRAVITY		0.002f

#define MINES_BBOX_WIDTH 15
#define MINES_BBOX_HEIGHT 9
#define MINES_BBOX_HEIGHT_DIE 9

#define MINES_STATE_IDLE 100
#define MINES_STATE_DIE 200

#define MINES_ANI_IDLING 0
#define MINES_ANI_DIE 1

#define MINES_SITEACTIVE_PLAYER		180

#define MINES_MAXHEALTH			1


class Mines : public Enemy
{
private:
	bool canExplosiving;
	bool isHitJason;
	bool died;
public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void Render();

	Mines(float x, float y, LPGAMEENTITY t);
	virtual void SetState(int state);
	void SelfDestroy();
};
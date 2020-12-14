#pragma once
#include <map>
#include "Timer.h"
#include "Player.h"
#include "Game.h"
#include "Grid.h"
#include <algorithm>
#include <assert.h>
#include "debug.h"
#include "Enemy.h"
#include "GateOverworld.h"


#define BIG_SOPHIA_BBOX_WIDTH		22
#define BIG_SOPHIA_BBOX_HEIGHT		32

#define BIG_SOPHIA_WALKING_SPEED			0.1f 

#define BIG_SOPHIA_STATE_IDLE					0
#define BIG_SOPHIA_STATE_WALKING_RIGHT			100
#define BIG_SOPHIA_STATE_WALKING_LEFT			200
#define BIG_SOPHIA_STATE_WALKING_TOP			300
#define BIG_SOPHIA_STATE_WALKING_BOTTOM			400
#define BIG_SOPHIA_STATE_DIE					500		
#define BIG_SOPHIA_STATE_AUTO_RUN				600

#define BIG_SOPHIA_ANI_IDLE_RIGHT				0
#define BIG_SOPHIA_ANI_IDLE_LEFT				1
#define BIG_SOPHIA_ANI_IDLE_TOP					2
#define BIG_SOPHIA_ANI_IDLE_BOTTOM				3
#define BIG_SOPHIA_ANI_WALKING_RIGHT			4
#define BIG_SOPHIA_ANI_WALKING_LEFT				5
#define BIG_SOPHIA_ANI_WALKING_TOP				6
#define BIG_SOPHIA_ANI_WALKING_BOTTOM			7
#define BIG_SOPHIA_ANI_DIE						8
#define LAST_FRAME_BIG_SOPHIA_DIE				15

#define MAX_GUNDAM						8

#define GATE_VERTICAL_LONG		125
#define GATE_HORIZONTAL_LONG	85


class Big_Sophia : public Player
{
	static Big_Sophia* instance;

	int level;
	bool isVerticalMove;
	float backup_x, backup_y;
	bool _isAutoRun;
	int directionAutoRun;
public:
	Big_Sophia (float x, float y, int health, int gundam);
	~Big_Sophia ();
	bool isAutoRun() { return _isAutoRun; };
	void AutoRun(int direction);
	void GetInfoBigSophia(int& direct, int& directY, float& playerx, float& playery, int& dame) { direct = nx; directY = ny; playerx = x; playery = y; dame = dam; }
	virtual void FireBullet(int type);
	virtual void GetPositionCenter(float&, float&) { x = this->x + BIG_SOPHIA_BBOX_WIDTH / 2; y = this->y + BIG_SOPHIA_BBOX_HEIGHT / 2; };
	virtual void Reset();
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

#pragma region GateOverworld
	//GateOverworld* gate;
	//GateOverworld* GetGate() { return gate; };
	//void ResetGate() { gate = NULL; };
	//bool GetGateColliding() { return GateColliding; };
	//void SetGateColliding(bool done) { GateColliding = done; };
#pragma endregion
};


#pragma once
#include "Entity.h"
#include <map>
#include "Timer.h"

#define SOPHIA_WALKING_SPEED_UNIT		0.0040f//0.010f
#define SOPHIA_WALKING_SPEED			0.1f 
#define SOPHIA_WALKING_SPEED_BONUS		0.003f//0.007f
#define SOPHIA_WALKING_ACC				0.00035f

#define SOPHIA_JUMP_SPEED_Y				0.223f
#define SOPHIA_JUMP_DEFLECT_SPEED		0.10f
#define SOPHIA_GRAVITY					0.0004f
#define SOPHIA_DIE_DEFLECT_SPEED		0.5f
#define SOPHIA_JASON_HEIGHT_GUN_FLIP	15.5

#pragma region State define
#define SOPHIA_STATE_IDLE				0
#define SOPHIA_STATE_WALKING_RIGHT		100
#define SOPHIA_STATE_WALKING_LEFT		200
#define SOPHIA_STATE_JUMP				300
#define SOPHIA_STATE_DIE				400
#define SOPHIA_STATE_GUN_UNFLIP			500
#define SOPHIA_STATE_OUT				600
#pragma endregion

#pragma region animation define

#define SOPHIA_ANI_JASON_IDLE_RIGHT					0
#define SOPHIA_ANI_JASON_IDLE_LEFT					1

#define SOPHIA_ANI_JASON_WALKING_RIGHT				2
#define SOPHIA_ANI_JASON_WALKING_LEFT				3

#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT			4
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT			5
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT		6
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT		7
#define SOPHIA_ANI_JASON_JUMP_UP_WALKING_RIGHT		8
#define SOPHIA_ANI_JASON_JUMP_UP_WALKING_LEFT		9
#define SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_RIGHT	10
#define SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_LEFT		11

#define SOPHIA_ANI_GUN_FLIP_RIGHT					12
#define SOPHIA_ANI_GUN_FLIP_LEFT					13

#define SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_1			14
#define SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_2			15
#define SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_3			16
#define SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_4			17

#define SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_1				18
#define SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_2				19
#define SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_3				20
#define SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_4				21

#define SOPHIA_JASON_ANI_DIE						22
#define SOPHIA_JASON_ANI_EJECTING					23
#pragma endregion

#define DURATION_X_TO_DIE	14
#define DURATION_Y_TO_DIE	30

#define HEIGHT_LEVER1 35


class JASON : public Entity
{
	int alpha;
	static JASON* instance;
	bool isJumpHandle;
	bool isGunFlipping = false;
	float start_x;				//initial position of Jason
	float start_y;

	float current_Jumpy;
	bool isPressJump;
	bool isPressFlipGun;
	bool isEjecting;
public:
	bool isDeath;
	bool isDoneDeath;
	bool isJumping = false;
	JASON(float x = 0.0f, float y = 0.0f);
	static JASON* GetInstance();

	void SetDirection(int d) { nx = d; };
	void SetState(int state);
	void SetPressSpace(bool isPress) { isPressJump = isPress; };
	void SetPressUp(bool a) { isPressFlipGun = a; }
	
	//void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	//void GetPositionCenter(float& x, float& y) { x = this->x + SOPHIA_JASON_BBOX_WIDTH / 2; y = this->y + SOPHIA_JASON_BBOX_HEIGHT / 2; }
	bool isGunFlippingg() { return isGunFlipping; }

	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL, vector<LPGAMEENTITY>* coEnemies = NULL);
	virtual void Render();

	void Setvx(float new_vx) { vx = new_vx; }
	void Setvy(float new_vy) { vy = new_vy; };
	float GetDy() { return dy; };
	float GetDx() { return dx; };
	void Reset();
};
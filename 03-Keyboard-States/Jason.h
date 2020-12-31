#pragma once
#include "Entity.h"
#include <map>
#include "Timer.h"
#include "Gate.h"
#include "Player.h"

#define SOPHIA_WALKING_SPEED_UNIT		0.0040f//0.010f
#define SOPHIA_WALKING_SPEED			0.1f 
#define SOPHIA_WALKING_SPEED_BONUS		0.003f//0.007f
#define SOPHIA_WALKING_ACC				0.00035f

#define SOPHIA_JUMP_SPEED_Y				0.235f
#define SOPHIA_JUMP_DEFLECT_SPEED		0.10f
#define SOPHIA_GRAVITY					0.0002f
#define SOPHIA_DIE_DEFLECT_SPEED		0.5f
#define SOPHIA_JASON_HEIGHT_GUN_FLIP	15.5

#pragma region State define
#define SOPHIA_STATE_IDLE				0
#define SOPHIA_STATE_DIE				100
#define SOPHIA_STATE_WALKING_RIGHT		200
#define SOPHIA_STATE_WALKING_LEFT		300
#define SOPHIA_STATE_JUMP				400
#define SOPHIA_STATE_GUN_UNFLIP			500
#define SOPHIA_STATE_OUT				600
#pragma endregion

#define DURATION_X_TO_DIE	14
#define DURATION_Y_TO_DIE	30
#define DISTANCE_TO_OUT		8
#define HEIGHT_LEVER1		50
#define MAX_HEALTH			8
#define SOPHIA_ANI_JASON_IDLE_RIGHT					0
#define SOPHIA_ANI_JASON_IDLE_LEFT					1

#define SOPHIA_ANI_JASON_WALKING_RIGHT				2
#define SOPHIA_ANI_JASON_WALKING_LEFT				3

#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT_1		4
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT_2		5
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT_3		6
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT_4		7
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT_1		8
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT_2		9
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT_3		10
#define SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT_4		11
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT_1		12
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT_2		13
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT_3		14
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT_4		15
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT_1		16
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT_2		17
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT_3		18
#define SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT_4		19
#define SOPHIA_ANI_JASON_JUMP_UP_WALKING_RIGHT		20
#define SOPHIA_ANI_JASON_JUMP_UP_WALKING_LEFT		21
#define SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_RIGHT	22
#define SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_LEFT		23

#define SOPHIA_ANI_GUN_FLIP_RIGHT					24
#define SOPHIA_ANI_GUN_FLIP_LEFT					25

#define SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_1			26
#define SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_2			27
#define SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_3			28
#define SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_4			29

#define SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_1				30
#define SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_2				31
#define SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_3				32
#define SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_4				33

#define SOPHIA_JASON_ANI_DIE						34
#define SOPHIA_JASON_ANI_EJECTING					35
#define SOPHIA_JASON_BBOX_WIDTH		26
#define SOPHIA_JASON_BBOX_HEIGHT	18

#define PLAYER_CHANGE_ALPHA_WHEN_DAMAGED 400

#define FALLING_VELOCITY_UPPER_LIMITATION 0.02f

class JASON : public Player
{

	static JASON* instance;
	bool isJumpHandle;
	bool isGunFlipping = false;

	bool isJumping = false;
	float current_Jumpy;
	bool isPressJump;
	bool isPressFlipGun;
	bool isEjecting;
	int burstFireModeBullets = 0;

	int noOfHomingMisslesWeaponLeft = 3;
	int noOfElectricWeaponLeft = 3;
	int noOfRocketsWeaponLeft = 3;
	EntityType specialBulletType;

public:
	JASON(float x, float y, int health, int gundam);
	JASON() {};
	static JASON* GetInstance();

	void GetNoOfBulletLeft(int& rocket, int& missles, int& electric) {
		rocket = noOfRocketsWeaponLeft;
		missles = noOfHomingMisslesWeaponLeft;
		electric = noOfElectricWeaponLeft;
	}
	int GetCurrentSpecialWeapon() { return specialBulletType; };
	void SetSpecialBulletType(EntityType _specialbullettype) { specialBulletType = _specialbullettype; };


	void SetState(int state);
	void SetPressSpace(bool isPress) { isPressJump = isPress; };
	void SetPressUp(bool a) { isPressFlipGun = a; }
	//void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	bool IsGunFlip() { return isGunFlipping; }
	bool IsJumping() { return isJumping; };
	void SetIsJumping(bool jump) { isJumping = jump; };
	void GetInfoForBullet(int& direct, bool& isTargetTop, float& playerx, float& playery) { direct = nx; isTargetTop = isGunFlipping; playerx = x; playery = y; }

#pragma region sceneswitching

#pragma endregion
	virtual void FireBullet(int mode);
	virtual void GetPositionCenter(float& x, float& y) { x = this->x + SOPHIA_JASON_BBOX_WIDTH / 2; y = this->y + SOPHIA_JASON_BBOX_HEIGHT / 2; }
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};
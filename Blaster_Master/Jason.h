#pragma once
#include "Entity.h"
#include <map>
#include "Timer.h"
#include "Gate.h"
#include "Player.h"

#define SOPHIA_WALKING_SPEED_UNIT		0.0040f//0.010f
#define JASON_WALKING_SPEED			0.1f 
#define JASON_WALKING_ACC				0.00035f

#define JASON_JUMP_SPEED_Y				0.235f	//0.235
#define JASON_GRAVITY					0.0004f
#define JASON_HEIGHT_GUN_FLIP	15.5

#pragma region State define
#define JASON_STATE_IDLE				0
#define JASON_STATE_DIE				100
#define JASON_STATE_WALKING_RIGHT		200
#define JASON_STATE_WALKING_LEFT		300
#define JASON_STATE_JUMP				400
#define JASON_STATE_GUN_UNFLIP			500
#define JASON_STATE_OUT				600
#pragma endregion

#define DURATION_X_TO_DIE	14
#define DURATION_Y_TO_DIE	30
#define DISTANCE_TO_OUT		8
#define HEIGHT_LEVER1		50	//50
#define MAX_HEALTH			8
#define JASON_ANI_IDLE_RIGHT					0
#define JASON_ANI_IDLE_LEFT					1

#define JASON_ANI_WALKING_RIGHT				2
#define JASON_ANI_WALKING_LEFT				3

#define JASON_ANI_JUMP_UP_IDLE_RIGHT_1		4
#define JASON_ANI_JUMP_UP_IDLE_RIGHT_2		5
#define JASON_ANI_JUMP_UP_IDLE_RIGHT_3		6
#define JASON_ANI_JUMP_UP_IDLE_RIGHT_4		7
#define JASON_ANI_JUMP_UP_IDLE_LEFT_1		8
#define JASON_ANI_JUMP_UP_IDLE_LEFT_2		9
#define JASON_ANI_JUMP_UP_IDLE_LEFT_3		10
#define JASON_ANI_JUMP_UP_IDLE_LEFT_4		11
#define JASON_ANI_JUMP_DOWN_IDLE_RIGHT_1		12
#define JASON_ANI_JUMP_DOWN_IDLE_RIGHT_2		13
#define JASON_ANI_JUMP_DOWN_IDLE_RIGHT_3		14
#define JASON_ANI_JUMP_DOWN_IDLE_RIGHT_4		15
#define JASON_ANI_JUMP_DOWN_IDLE_LEFT_1		16
#define JASON_ANI_JUMP_DOWN_IDLE_LEFT_2		17
#define JASON_ANI_JUMP_DOWN_IDLE_LEFT_3		18
#define JASON_ANI_JUMP_DOWN_IDLE_LEFT_4		19
#define JASON_ANI_JUMP_UP_WALKING_RIGHT		20
#define JASON_ANI_JUMP_UP_WALKING_LEFT		21
#define JASON_ANI_JUMP_DOWN_WALKING_RIGHT	22
#define JASON_ANI_JUMP_DOWN_WALKING_LEFT		23

#define JASON_ANI_GUN_FLIP_RIGHT					24
#define JASON_ANI_GUN_FLIP_LEFT					25

#define JASON_ANI_GUN_FLIP_IDLE_RIGHT_1			26
#define JASON_ANI_GUN_FLIP_IDLE_RIGHT_2			27
#define JASON_ANI_GUN_FLIP_IDLE_RIGHT_3			28
#define JASON_ANI_GUN_FLIP_IDLE_RIGHT_4			29

#define JASON_ANI_GUN_FLIP_IDLE_LEFT_1				30
#define JASON_ANI_GUN_FLIP_IDLE_LEFT_2				31
#define JASON_ANI_GUN_FLIP_IDLE_LEFT_3				32
#define JASON_ANI_GUN_FLIP_IDLE_LEFT_4				33

#define JASON_ANI_DIE						34
#define JASON_ANI_EJECTING					35
#define JASON_BBOX_WIDTH		26
#define JASON_BBOX_HEIGHT	18

#define PLAYER_CHANGE_ALPHA_WHEN_DAMAGED 400

#define FALLING_VELOCITY_UPPER_LIMITATION 0.02f

#define BURST_FIRE_DELAY	100
class JASON : public Player
{
private:
	static JASON* instance;
	bool isJumpHandle;
	bool isGunFlipping = false;

	bool isJumping = false;
	float current_Jumpy;
	bool isPressJump;
	bool isPressFlipGun;
	bool isEjecting;
	int burstFireModeBullets = 0;

	
	Timer* burstFireTimer = new Timer(BURST_FIRE_DELAY);
public:
	JASON(float x, float y, int health, int gundam);
	JASON() {};
	static JASON* GetInstance();

	void SetState(int state);
	void SetPressSpace(bool isPress) { isPressJump = isPress; };
	void SetPressUp(bool a) { isPressFlipGun = a; }
	bool IsGunFlip() { return isGunFlipping; }
	bool IsJumping() { return isJumping; };
	void SetIsJumping(bool jump) { isJumping = jump; };
	void GetInfoForBullet(int& direct, bool& isTargetTop, float& playerx, float& playery) { direct = nx; isTargetTop = isGunFlipping; playerx = x; playery = y; }
	virtual void FireBullet(int mode);
	virtual void GetPositionCenter(float& x, float& y) { x = this->x + JASON_BBOX_WIDTH / 2; y = this->y + JASON_BBOX_HEIGHT / 2; }
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};
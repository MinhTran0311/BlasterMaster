#include "Entity.h"
#include <map>
#include "Timer.h"

#pragma once
#define ANIMATION_SET_SMALL_SOPHIA	2

#define SOPHIA_GRAVITY						0.0004f
#define SMALL_SOPHIA_WALKING_SPEED			0.07f 
#define SMALL_SOPHIA_WALKING_ACC			0.00015f
#define SMALL_SOPHIA_CRAWLING_SPEED			0.03f 
#define SMALL_SOPHIA_JUMP_SPEED_Y			0.15f

#define SOPHIA_ANI_SMALL_IDLE_RIGHT				0
#define SOPHIA_ANI_SMALL_WALKING_RIGHT			1
#define SOPHIA_ANI_SMALL_IDLE_CRAWL_RIGHT		2
#define SOPHIA_ANI_SMALL_WALKING_CRAWL_RIGHT		3
#define SOPHIA_ANI_SMALL_DIE						3
#define SOPHIA_ANI_SMALL_JUMP					4

#define SMALL_SOPHIA_STATE_IDLE			    0
#define SMALL_SOPHIA_STATE_WALKING_RIGHT		100
#define SMALL_SOPHIA_STATE_WALKING_LEFT		200
#define SMALL_SOPHIA_STATE_CRAWLING_RIGHT	300
#define SMALL_SOPHIA_STATE_CRAWLING_LEFT		400
#define SMALL_SOPHIA_STATE_JUMP				500
#define SMALL_SOPHIA_STATE_DIE				600
#define SMALL_SOPHIA_STATE_IN				700
#define SMALL_SOPHIA_STATE_OUT				800
#define SMALL_SOPHIA_STATE_CRAWL				900
#define SMALL_SOPHIA_STATE_CRAWL_STOP		1000

#define MAX_HEALTH						8


#define SMALL_SOPHIA_BBOX_WIDTH				10
#define SMALL_SOPHIA_BBOX_HEIGHT				16
#define SMALL_SOPHIA_CRAWL_BBOX_WIDTH		16
#define SMALL_SOPHIA_CRAWL_BBOX_HEIGHT		10


#define PLAYER_IMMORTAL_DURATION	1000
#define HIGH_LEVER1 43

#define LAST_FRAME_DIE	13

class Small_Sophia : public Entity
{
public:
	bool isDeath;
	bool isDoneDeath;
	static Small_Sophia* instance;

	int level;
	int untouchable;
	bool isJumping = false;
	bool isGunFlipping = false;
	bool isJumpHandle;
	bool isImmortaling;
	int alpha;
	DWORD untouchable_start;

	Timer* immortalTimer = new Timer(PLAYER_IMMORTAL_DURATION);

	float start_x;			// initial position of Mario at scene
	float start_y;


	float backup_JumpY;
	bool isPressJump;
	bool isPressFlipGun;
	bool isCrawl;

public:
	Small_Sophia(float x = 0.0f, float y = 0.0f);
	static Small_Sophia* GetInstance();

	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL, vector<LPGAMEENTITY>* coEnemies = NULL);
	virtual void Render();

	//Immortal
	bool IsImmortaling() { return isImmortaling; }
	void SetImmortaling(bool immo) { isImmortaling = immo; }
	void StartImmortalingTimer() { immortalTimer->Start(); }

	void SetDirection(int d) { nx = d; }
	void SetState(int state);
	void SetPressSpace(bool isPress) { isPressJump = isPress; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void GetPositionCenter(float& x, float& y) { x = this->x + SMALL_SOPHIA_BBOX_WIDTH / 2; y = this->y + SMALL_SOPHIA_BBOX_HEIGHT / 2; }
	bool GetIsCrawl() { return isCrawl; }
	void SetIsCrawl(bool crawl) { isCrawl = crawl; }
	void Setvx(float vx) { vx = vx; }
	void Setvy(float vy) { vy = vy; }
	float GetDy() { return dy; }
	float Getvy() { return vy; }
	void Reset();
	void GetInfoForBullet(int& direct, float& playerx, float& playery) { direct = nx; playerx = x; playery = y; }

	//Bullet* GetPlayerMainBullet() { return mainBullet; }
	void SetInjured(int dame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


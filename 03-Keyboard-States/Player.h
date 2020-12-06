#pragma once
#include "Entity.h"
#include <map>
#include "Timer.h"
#include "Gate.h"

#define PLAYER_IMMORTAL_DURATION	1000
#define PLAYER_CAN_FIRE_DURATION	120

class Player :public Entity
{
	static Player* instance;
protected:
	EntityType _PlayerType;

	int alpha=255;
	bool isDeath;
	bool isDoneDeath;

	float start_x;				//initial position of Jason
	float start_y;

	//imortall
	int untouchable;
	bool isImmortaling;
	DWORD untouchable_start;
	Timer* immortalTimer = new Timer(PLAYER_IMMORTAL_DURATION);
	bool canFire;
	Timer* FireTimer = new Timer(PLAYER_CAN_FIRE_DURATION);
	bool canChangeAlpha = false;
public:
	Player();
	~Player();
	static Player* GetInstance();
	void SetState(int state) {Entity::SetState(state);};


	//get set functions

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Setvx(float new_vx) { vx = new_vx; }
	void Setvy(float new_vy) { vy = new_vy; };
	float Getvy() { return vy; };
	float GetDy() { return dy; };
	float GetDx() { return dx; };
	void SetIsDoneDeath(bool donedeath) { isDoneDeath = donedeath; };
	void SetIsDeath(bool death) { isDeath = death; };

	EntityType GetPlayerType() { return _PlayerType; };
	void SetPlayerType(EntityType type) { _PlayerType = type; };
	void SetInjured(int dame);
	//Immortal
	bool IsImmortaling() { return isImmortaling; }
	void SetImmortaling(bool immo) { isImmortaling = immo; }
	void StartImmortalingTimer() { immortalTimer->Start(); }

	virtual void FireBullet(int mode) {};
	virtual void GetPositionCenter(float&, float&) {};
	virtual void Reset() {};
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	virtual void Render() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};

};
typedef Player* LPGAMEPLAYER;

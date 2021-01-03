#pragma once
#include "Entity.h"
#include <map>
#include "Timer.h"
#include "Gate.h"
#include <algorithm>
#include <assert.h>
#include "debug.h"
#include "Game.h"
#include "Worms.h"
#include "Gate.h"
#include "PlayScene.h"
#include "Grid.h"
#include "JasonBullet.h"
#include "JasonRocket.h"
#include "InjuringBrick.h"
#include "GadBrick.h"
#include "ElectricBullet.h"
#include "HomingMissles.h"
#include "Big_Sophia.h"
#include "Item.h"
#include "PlayerHandler.h"

#define PLAYER_IMMORTAL_DURATION	1000
#define PLAYER_CAN_FIRE_DURATION	120
#define PLAYER_CHANGE_ALPHA		400

#define PLAYER_STATE_IDLE	0
#define PLAYER_STATE_DIE	100
class Player :public Entity
{

protected:
	static Player* instance;
	EntityType _PlayerType;

	int alpha = 255;
	bool isDeath = false;
	bool isDoneDeath=false;

	float start_x;				//initial position of Jason
	float start_y;

	bool GateColliding;

	bool isInjured=false;

	//imortall
	int untouchable;
	bool isImmortaling = false;
	DWORD untouchable_start;
	Timer* immortalTimer = new Timer(PLAYER_IMMORTAL_DURATION);
	bool canFire;
	Timer* FireTimer = new Timer(PLAYER_CAN_FIRE_DURATION);
	Timer* changeAlphaTimer = new Timer(PLAYER_CHANGE_ALPHA);
	bool canChangeAlpha = false;
	LPGAMEENTITY gate;
	bool _isAutoRun;

	//bullet
	//int noOfHomingMisslesWeaponLeft = 3;
	//int noOfElectricWeaponLeft = 3;
	//int noOfRocketsWeaponLeft = 3;
	//EntityType specialBulletType;
public:
	Player();
	~Player();
	static Player* GetInstance();
	//gate
	LPGAMEENTITY GetGate() { return gate; };
	void ResetGate() { gate = NULL; };
	bool GetGateColliding() { return GateColliding; };
	void SetGateColliding(bool done) { GateColliding = done; };
	void SetState(int state) { Entity::SetState(state); };
	void changeAlpha();
	//get set functions
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Setvx(float new_vx) { vx = new_vx; }
	void Setvy(float new_vy) { vy = new_vy; };
	float Getvy() { return vy; };
	float GetDy() { return dy; };
	float GetDx() { return dx; };
	void SetIsDoneDeath(bool donedeath) { isDoneDeath = donedeath; };
	void SetIsDeath(bool death) { isDeath = death; };
	bool IsDoneDeath(){return isDoneDeath;};
	EntityType GetPlayerType() { return _PlayerType; };
	void SetPlayerType(EntityType type) { _PlayerType = type; };
	void SetInjured(int dame);
	//Immortal
	bool IsImmortaling() { return isImmortaling; }
	void SetImmortaling(bool immo) { isImmortaling = immo; }
	void StartImmortalingTimer() { immortalTimer->Start(); }

	virtual void FireBullet(int mode) {};
	virtual void GetPositionCenter(float&, float&) {};
	virtual void Reset(int health, int gundam);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL);
	virtual void Render() {};
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};

	void EnemyBulletHitPlayer(int);
	void CollideWithObject(LPGAMEENTITY object);
	void CollisionHandle(vector<LPGAMEENTITY>* coObjects);

	bool isAutoRun() { return _isAutoRun; };
	void SetAutoRun(int _auto) { _isAutoRun = _auto; };
	//void GetNoOfBulletLeft(int& rocket, int& missles, int& electric) {
	//	rocket = noOfRocketsWeaponLeft;
	//	missles = noOfHomingMisslesWeaponLeft;
	//	electric = noOfElectricWeaponLeft;
	//}
	//int GetCurrentSpecialWeapon() { return specialBulletType; };
	//void SetSpecialBulletType(EntityType _specialbullettype) { specialBulletType = _specialbullettype; };
};
typedef Player* LPGAMEPLAYER;

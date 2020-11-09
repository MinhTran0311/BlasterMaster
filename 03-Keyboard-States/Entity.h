#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
#include "debug.h"

using namespace std;

class Entity;
typedef Entity* LPGAMEENTITY;

//struct CCollisionEvent;
//typedef CCollisionEvent* LPCOLLISIONEVENT;
//struct CCollisionEvent
//{
//	LPGAMEENTITY obj;
//	float t, nx, ny;
//
//	float dx, dy;		// *RELATIVE* movement distance between this object and obj
//
//	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEENTITY obj = NULL)
//	{
//		this->t = t;
//		this->nx = nx;
//		this->ny = ny;
//		this->dx = dx;
//		this->dy = dy;
//		this->obj = obj;
//	}
//
//	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
//	{
//		return a->t < b->t;
//	}
//};

class Entity
{
public:

	float x; 
	float y;

	float vx;
	float vy;

	float dx, dy;
	int nx;	 
	int ny;
	int health = 1;
	int gunDam;
	int state;									

	//static vector<LPANIMATION> animations;

	//int bbARGB;

	DWORD dt;
	EntityType tag;
	LPANIMATION_SET animationSet;

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }
	float Getx() { return x; }
	float Gety() { return y; }

	/*int GetHeight{ return texture}*/

	int GetHealth() { return health; }
	//void SetHealth(int value) { health = value; }
	//int GetgunDam() { return gunDam; }
	//void SetgunDam(int value) { gunDam = value; }
	//void AddHealth(int BonusHealth) { health += BonusHealth; }
	//void AddgunDam(int BonusgunDam) { gunDam += BonusgunDam; }

	virtual void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }

	EntityType GetType() { return tag; }

	void SetAnimationSet(LPANIMATION_SET ani_set) { animationSet = ani_set; }


	Entity();
	Entity(float x, float y);

	virtual void Update(DWORD dt);
	virtual void Render();
	~Entity();
};
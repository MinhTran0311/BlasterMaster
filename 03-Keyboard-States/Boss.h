#pragma once
#include "Enemy.h"
#include "Vec2.h"
#include <cmath>
#include "global.h"
#include "Grid.h"
#include "Bullet.h"
#include "BossBullet.h"
#include "Timer.h"
#include "vector"
#define BOSS_WALKING_SPEED 0.05f;

#define BOSS_BBOX_WIDTH 60
#define BOSS_BBOX_HEIGHT 60
#define BOSS_BBOX_HEIGHT_DIE 9

#define BOSS_STATE_WALKING_LEFT 100
#define BOSS_STATE_WALKING_RIGHT 200
#define BOSS_STATE_DIE 300

#define BOSS_ANI_WALKING_LEFT 0
#define BOSS_ANI_WALKING_RIGHT 1
#define BOSS_ANI_DIE	2

#define BOSS_BULLET_BURST_DELAY	5000
#define BOSS_BULLET_SHOOT_ONE_DELAY 500

class CBoss : public Enemy
{
	int nx;
	float startX;
	float startY;
	int bulletCount=0;
	bool isWaitingToShoot = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
public: 
	CBoss(float xPos, float yPos,LPGAMEENTITY t);
	//void SetTarget(LPGAMEENTITY _target) { target = _target; };

private:
	class BossClawSection : public Enemy {
		Vec2 startPoint;
		Vec2 endPoint;
		float Angle;
	public:
		
		void setStartPoint(Vec2 sp);
		Vec2 getEndpoint() { return endPoint; };
		float getAngle() { return this->Angle; };
		const float  SectionLength = 22;
		void calculateEndpoint();
		void Follow(float x, float y);
		void Follow(Vec2& target);
		void Follow(BossClawSection& target); // Follow the start point of target
		BossClawSection(int anisetid);
		BossClawSection();
		BossClawSection(Vec2 start, float angle);
		virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
		virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
		virtual void Render();

	};
public:

	CBoss();
	virtual void SetState(int state);
	void SetStartPosition(float x, float y);
	void getLeftClawPos(float& x, float& y);
	void getRightClawPos(float& x, float& y);
private:
	Vec2 Target1 = Vec2(350, 150);
	Vec2 Target2 = Vec2(150, 150);
	Vec2 Speed = Vec2(1.5f, 1.5);
	void updateTarget1();
	void updateTarget2();
	void Init();
	BossClawSection BigClawLeft;
	BossClawSection BigClawRight;
	BossClawSection LeftArm[4];
	BossClawSection RightArm[4];
	void Shoot();

	Timer* bursttimer = new Timer(BOSS_BULLET_BURST_DELAY);
	Timer* shoottimer = new Timer(BOSS_BULLET_SHOOT_ONE_DELAY);
	


	int indexTarget2 = 0;
	Vec2 nextTarget2;
	Vec2 rList[6];

	int indexTarget1 = 0;
	Vec2 nextTarget1;
	Vec2 lList[19];
};
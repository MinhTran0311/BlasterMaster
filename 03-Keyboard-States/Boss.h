#pragma once
#include "Enemy.h"
#include "Vec2.h"
#include <cmath>
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

class CBoss : public Enemy
{
	int nx;
	float startX;
	float startY;
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
public: CBoss(float x, float y);

private:
	class BossClawSection : public Enemy {
		Vec2 startPoint;
		Vec2 endPoint;
		float Angle;
	public:
		
		void setStartPoint(Vec2 sp);
		Vec2 getEndpoint() { return endPoint; };
		float getAngle() { return this->Angle; };
		static float constexpr SectionLength = 20;
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
	Vec2 Target2 = Vec2(410, 150);
	Vec2 Speed = Vec2(1.5f, 1.5f);
	Vec2 Speed2 = Vec2(-1.5f, -1.5f);
	void updateTarget1();
	void updateTarget2();
	void Init();
	BossClawSection BigClawLeft;
	BossClawSection BigClawRight;
	BossClawSection LeftArm[4];
	BossClawSection RightArm[4];
	void Shoot();
	int counter = 0;
	int counter1 = 0;
	int counter2 = 300;
	int counter3 = 600;
	int counter4 = 900;
};
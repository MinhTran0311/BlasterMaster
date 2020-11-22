//#pragma once
//#include "Entity.h"
//#define BULLET_SPEED 0.1f
//
//#define BULLET_HORIZONTAL_BBOX_WIDTH 24
//#define BULLET_HORIZONTAL_BBOX_HEIGHT 8
//
//#define BULLET_VERTICAL_BBOX_WIDTH 8
//#define BULLET_VERTICAL_BBOX_HEIGHT 24
//
<<<<<<< HEAD
//#define BULLET_FLYING_SPACE 150
=======
//#define BULLET_STATE_FLYING_SPACE 150
>>>>>>> main
//
//#define BULLET_STATE_FLYING_LEFT 100
//#define BULLET_STATE_FLYING_RIGHT 200
//#define	BULLET_STATE_FLYING_UP	300
//#define BULLET_STATE_DIE		400
//
//#define BULLET_ANI_FLYING_LEFT_LV1	0
//#define BULLET_ANI_FLYING_RIGHT_LV1	1
//#define BULLET_ANI_FLYING_UP_LV1	2
//#define BULLET_ANI_FLYING_LEFT_LV2	3
//#define BULLET_ANI_FLYING_RIGHT_LV2	4
//#define BULLET_ANI_FLYING_UP_LV2	5
//
//
//#define BULLET_ANI_DESTROYED	6
//
//
//class CBullet : public Entity
//{
//	float startPositionX;
//	float startPositionY;
//	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
//	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
//	virtual void Render();
//	int level;
//
//
//public:
//	CBullet(int level, int state);
//	virtual void SetState(int state);
//	virtual void SetStartPositon(float x, float y);
//	int GetState();
//};
//
#pragma once
#include "Entity.h"

#define DISTANCE_FIRING_WIDTH	10
#define DISTANCE_FIRING_HEIGHT	3
#define DISTANCE_BLOWING_UP		4
#define BULLET_SPEED			0.2f

<<<<<<< HEAD

=======
#define BULLET_STATE_FLYING			100
#define BULLET_STATE_BLOW_UP		200
#define BULLET_STATE_FLYING_TOP		300
#define BULLET_STATE_DISAPPEAR		400
>>>>>>> main
class Bullet;
typedef Bullet* LPBULLET;
class Bullet : public Entity
{
public:
	int totalTime;
	int x_Start_Pos, y_Start_Pos;
	bool able_firing;
<<<<<<< HEAD
	int dam;
	int isAimingTop;
	int alpha, typeBullet;
	bool  isHitBrick, isHitEnemy;
	float timeDelayed, timeDelayedMax;
	float equation_a, equation_b;
	LPGAMEENTITY targetObject;
=======
	int isAimingTop;
	int alpha;
	int typeBullet;
	bool  isHitBrick, isHitEnemy;
	float timeDelayed, timeDelayedMax;

>>>>>>> main
	// phương thức
	Bullet();
	~Bullet();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects) { Entity::Update(dt, coObjects); };
	virtual void Render() {};

	//void Fire(int type,
	//	int direct,
	//	int isGunFlip,
	//	float posX,
	//	float posY) {
	//	typeBullet = type;
	//	nx = direct;
	//	isAimingTop = isGunFlip;
	//	x = posX + DISTANCE_FIRING_WIDTH;
	//	y = posY + DISTANCE_FIRING_HEIGHT;
	//	alpha = 255; able_firing = false;
	//	isHitBrick = 0;
	//	isHitEnemy = 0;
	//}
	//void BigSophiaFire(int direct, int directY, float posX, float posY, int dame) {
	//	nx = direct;
	//	ny = directY;
	//	x = posX + DISTANCE_FIRING_WIDTH;
	//	y = posY + DISTANCE_FIRING_HEIGHT;
	//	alpha = 255; able_firing = false;
	//	isHitBrick = 0;
	//	isHitEnemy = 0;
	//	dam = dame;
	//	x_Start_Pos = posX + DISTANCE_FIRING_WIDTH;
	//	y_Start_Pos = posY + DISTANCE_FIRING_HEIGHT;
	//	totalTime = 0;
	//}
	void ResetDelay() { timeDelayed = 0; }
	bool GetisActive() { return isActive; }
	//void SetIsDone(bool isdone) { isDone = isdone; }
	int GetDamage() { return dam; }
<<<<<<< HEAD
=======
	int GetBulletType() { return typeBullet; };
>>>>>>> main
};

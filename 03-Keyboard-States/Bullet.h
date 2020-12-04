#pragma once
#include "Entity.h"

#define DISTANCE_FIRING_WIDTH	10
#define DISTANCE_FIRING_HEIGHT	3
#define DISTANCE_BLOWING_UP		4
#define BULLET_SPEED			0.2f

#define BULLET_STATE_FLYING			100
#define BULLET_STATE_BLOW_UP		200
#define BULLET_STATE_FLYING_TOP		300
#define BULLET_STATE_DISAPPEAR		400

class Bullet;
typedef Bullet* LPBULLET;
class Bullet : public Entity
{
public:
	int totalTime;
	int x_Start_Pos, y_Start_Pos;
	bool able_firing;
	int isAimingTop;
	int alpha;
	int typeBullet;
	bool  isHitBrick, isHitEnemy;
	float timeDelayed, timeDelayedMax;

	// phương thức
	Bullet();
	~Bullet();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
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
	int GetBulletType() { return typeBullet; };
};

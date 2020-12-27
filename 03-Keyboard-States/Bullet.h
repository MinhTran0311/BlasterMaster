#pragma once
#include "Entity.h"

#define DISTANCE_FIRING_WIDTH	5
#define DISTANCE_FIRING_HEIGHT	3
#define DISTANCE_BLOWING_UP		4
#define BULLET_SPEED			0.2f

#define BULLET_STATE_FLYING			0
#define BULLET_STATE_HIT_BRICK		100
#define BULLET_STATE_ENEMY			200

class Bullet;
typedef Bullet* LPBULLET;
class Bullet : public Entity
{

public:
	int totalTime;
	float x_Start_Pos, y_Start_Pos;
	bool able_firing;
	int isAimingTop;
	int alpha;
	EntityType typeBullet;
	float bullet_speed;
	bool  isHitBrick, isHitEnemy, isHitJason;
	float timeDelayed, timeDelayedMax;

	// phương thức
	Bullet();
	~Bullet();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render() {};
	virtual void SetState(int state);

	void HandlePlayerBulletCollision(vector<LPGAMEENTITY>* coObjects);
	void ResetDelay() { timeDelayed = 0; }
	int GetDamage() { return dam; }
	int GetBulletType() { return typeBullet; };
};

#pragma once
#include "Entity.h"
#include "Jason.h"
#include "Bullet.h"

#define ROCKET_FLYING_SPEED 0.08f

#define ROCKET_BBOX_WIDTH	15
#define ROCKET_BBOX_HEIGHT 6

#define BULLET_JASON_STATE_FLYING						0
#define BULLET_JASON_STATE_HIT_BRICK					100
#define BULLET_JASON_STATE_HIT_ENEMY					200

#define BULLET_JASON_DELAY			2000

#define ROCKET_ANI_FLYING 0
#define MAX_FLYING_RANGE	700.0f
class JasonRocket : public Bullet
{
public:
	JasonRocket(float xPos, float yPos, int direct_x);
	~JasonRocket();
	pair<float, float> targetPos;
	Entity* targetObject;
	vector<LPGAMEENTITY> *objects;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();

	float GetDistance(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);
	bool identifyTarget(vector<LPGAMEENTITY>* objects);
	//void setTargetObjects(vector<LPGAMEENTITY>* objects);
	Entity* FirstTarget(vector<LPGAMEENTITY>* objects);
	virtual void SetState(int state);
	void SetNavigation();
	void SetTargetObject(Entity* object);
};

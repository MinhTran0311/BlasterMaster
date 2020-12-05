#pragma once
#include "Entity.h"

#define SOFT_BRICK_BBOX_WIDTH  15
#define SOFT_BRICK_BBOX_HEIGHT 17

#define EXPLOSION_BBOX_WIDTH 19

#define SOFTBRICK_STATE_IDLE 0
#define SOFTBRICK_STATE_EXPLOSION 100

#define SOFTBRICK_ANI_IDLE 0
#define SOFTBRICK_ANI_EXPLOSION 1

class SoftBrick : public Entity
{
public:
	float frameW, frameH;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void GetCenterBoundingBox(float& x, float& y);
	virtual void SetState(int state);
	SoftBrick(int x, int y);
	~SoftBrick();
};
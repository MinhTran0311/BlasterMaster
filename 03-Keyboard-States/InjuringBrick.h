#pragma once
#include "Entity.h"

//#define BRICK_BBOX_WIDTH  16
//#define BRICK_BBOX_HEIGHT 16

class InjuringBrick : public Entity
{
public:
	float frameW, frameH;

	int GetDamage() { return dam; };

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	InjuringBrick(int x, int y, float frameW, float frameH);
	~InjuringBrick();
};

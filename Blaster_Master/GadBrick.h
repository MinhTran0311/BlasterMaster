#pragma once
#include "Entity.h"

//#define BRICK_BBOX_WIDTH  16
//#define BRICK_BBOX_HEIGHT 16

class GadBrick : public Entity
{
public:
	float frameW, frameH;
	
	int GetDamage() { return dam; };

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	GadBrick(int x, int y, float frameW, float frameH);
	~GadBrick();
};

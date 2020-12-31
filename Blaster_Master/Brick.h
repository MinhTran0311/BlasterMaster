#pragma once
#include "Entity.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class Brick : public Entity
{
private: 
	float frameW, frameH;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetBrickWidth() { return frameW; };
	float GetBrickHeight() { return frameH; };

	Brick(int x, int y, float frameW, float frameH);
	~Brick();
};
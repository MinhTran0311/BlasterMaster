#pragma once
#include "Entity.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class Brick : public Entity
{
private: 
	float width, height;
	bool isCanShootThrought = false;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetBrickWidth() { return width; };
	float GetBrickHeight() { return height; };
	bool IsCanShootThrought() { return isCanShootThrought; };

	Brick(int x, int y, float frameW, float frameH, int canshootthrought=0);
	~Brick();
};
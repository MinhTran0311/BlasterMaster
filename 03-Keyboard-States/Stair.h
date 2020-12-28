#pragma once
#include "Entity.h"

#define STAIR_BBOX_WIDTH				8
#define STAIR_BBOX_HEIGHT				180

class Stair : public Entity
{
private:
	int width, height;
public:
	Stair(float x, float y, int width, int height);
	~Stair();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
};


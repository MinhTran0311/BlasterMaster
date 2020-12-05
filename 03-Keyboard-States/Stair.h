#pragma once
#include "Entity.h"

#define STAIR_BBOX_WIDTH				8
#define STAIR_BBOX_HEIGHT				180

class Stair : public Entity
{
public:
	Stair(float x, float y);
	~Stair();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();
};


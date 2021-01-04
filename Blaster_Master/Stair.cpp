#include "Stair.h"


Stair::Stair(float xPos, float yPos, int _width, int _height)
{
	tag = EntityType::TAG_STAIRS;
	this->x = xPos;
	this->y = yPos;
	width = _width;
	height = _height;
}
Stair::~Stair() {}

void Stair::Render()
{
	//RenderBoundingBox();
}

void Stair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

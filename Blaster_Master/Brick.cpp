#include "Brick.h"

void Brick::Render()
{
	RenderBoundingBox();
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + frameW;
	b = y + frameH;
}

Brick::Brick(int x, int y, float frameW, float frameH)
{
	tag = EntityType::TAG_BRICK;
	this->SetPosition(x, y);
	this->frameH = frameH;
	this->frameW = frameW;
}

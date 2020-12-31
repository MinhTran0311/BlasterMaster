#include "InjuringBrick.h"

void InjuringBrick::Render()
{
	RenderBoundingBox();
}

void InjuringBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + frameW;
	b = y + frameH;
}

InjuringBrick::InjuringBrick(int x, int y, float frameW, float frameH)
{
	bbARGB = 120;

	tag = EntityType::TAG_INJURING_BRICK;
	this->SetPosition(x, y);
	this->frameH = frameH;
	this->frameW = frameW;
	dam = 1;
}

InjuringBrick::~InjuringBrick()
{
}

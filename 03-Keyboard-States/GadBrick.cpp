#include "GadBrick.h"

void GadBrick::Render()
{
	RenderBoundingBox();
}

void GadBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + frameW;
	b = y + frameH;
}

GadBrick::GadBrick(int x, int y, float frameW, float frameH)
{
	bbARGB = 120;
	tag = EntityType::TAG_GAD_BRICK;
	this->SetPosition(x, y);
	this->frameH = frameH;
	this->frameW = frameW;
	dam = 1;
}

GadBrick::~GadBrick()
{
}

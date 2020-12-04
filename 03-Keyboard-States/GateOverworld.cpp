#include "GateOverworld.h"

GateOverworld::GateOverworld(float x, float y, float newXPosGo, float newXPosBack, float newYPosGo, float newYPosBack, int directCam)
{
	tag = EntityType::TAG_GATE_OVERWORLD;
	this->x = x;
	this->y = y;
	xPosGo = newXPosGo;
	xPosBack = newXPosBack;
	yPosGo = newYPosGo;
	yPosBack = newYPosBack;
	directionCam = directCam;
}

GateOverworld::~GateOverworld()
{
}

void GateOverworld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = this->x;
	top = this->y;
	if (directionCam == 1)
	{
		right = left + GATE_OVERWORLD_HORIZONTAL_BBOX_WIDTH;
		bottom = top + GATE_OVERWORLD_HORIZONTAL_BBOX_HEIGHT;
	}
	else
	{
		right = left + GATE_OVERWORLD_VERTICAL_BBOX_WIDTH;
		bottom = top + GATE_OVERWORLD_VERTICA_BBOX_NEW_HEIGHT;
	}
}

void GateOverworld::Render()
{
	RenderBoundingBox();
}

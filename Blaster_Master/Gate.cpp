#include "Gate.h"


Gate::Gate(float x, float y, int switchId, float playerx, float playery, int playerstate, int flPlayer, int camX, int camY, int direction)
{
	tag = EntityType::TAG_GATE;
	this->x = x;
	this->y = y;
	gateSwitchSceneId = switchId;
	newPlayerx = playerx;
	newPlayery = playery;
	newPlayerState = playerstate;
	isCamFollowPlayer = flPlayer;
	camPosX = camX;
	camPosY = camY;
	moveDirection = direction;
}

Gate::~Gate()
{
}

void Gate::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GATE_BBOX_WIDTH;
	bottom = y + GATE_BBOX_HEIGHT;
}

void Gate::Render()
{
	RenderBoundingBox();
}

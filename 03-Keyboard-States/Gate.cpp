#include "Gate.h"


Gate::Gate(float x, float y, int switchId, float playerx, float playery, int playerstate, bool rsCam, EntityType type, int camX, int camY)
{
	//this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_GATE));
	tag = EntityType::TAG_GATE;
	this->x = x;
	this->y = y;
	gateSwitchSceneId = switchId;
	newPlayerx = playerx;
	newPlayery = playery;
	newPlayerState = playerstate;
	isNeedResetCam = rsCam;
	typePlayer = type;
	camPosX = camX;
	camPosY = camY;
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

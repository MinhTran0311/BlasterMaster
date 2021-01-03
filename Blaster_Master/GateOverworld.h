#pragma once
#include "Entity.h"

#define GATE_OVERWORLD_HORIZONTAL_BBOX_WIDTH			54
#define GATE_OVERWORLD_HORIZONTAL_BBOX_HEIGHT			30
#define GATE_OVERWORLD_VERTICAL_BBOX_WIDTH				20
#define GATE_OVERWORLD_VERTICA_BBOX_NEW_HEIGHT			62

class GateOverworld : public Entity
{
private:
	int directionCam;	
	float xPosGo, yPosGo, xPosBack, yPosBack;
public:
	GateOverworld(float x, float y, float intoXGo, float intoXBack, float intoYGo, float intoYBack, int directCam);
	~GateOverworld();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL) {};
	virtual void Render();

	float GetXPosGo() { return xPosGo; };
	float GetXPosBack() { return xPosBack; };
	float GetYPosGo() { return yPosGo; };
	float GetYPosBack() { return yPosBack; };
	int GetCamDirection() { return directionCam; };
};


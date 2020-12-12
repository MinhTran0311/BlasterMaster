#pragma once
#include "Entity.h"

#define GATE_BBOX_WIDTH				30
#define GATE_BBOX_HEIGHT			50

class Gate : public Entity
{
	int gateSwitchSceneId;	//Gate nay se dan~ den scene nao
public:
	bool directionCam;
	bool isNeedResetCam;
	int newPlayerx, newPlayery;
	int newPlayerState;
	int camPosX, camPosY;

	Gate(float x, float y, int switchId, float playerx, float playery, int playerState, bool rsCam, int camX, int camY);


	~Gate();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();

	int GetIdScene() { return gateSwitchSceneId; }
};


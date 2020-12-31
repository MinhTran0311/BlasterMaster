#pragma once
#include "Entity.h"

#define GATE_BBOX_WIDTH				20
#define GATE_BBOX_HEIGHT			30

class Gate : public Entity
{
private:
	int gateSwitchSceneId;	//Gate nay se dan~ den scene nao

	bool isCamFollowPlayer;
	int newPlayerx, newPlayery;
	int newPlayerState;
	int camPosX, camPosY;
public:
	Gate(float x, float y, int switchId, float playerx, float playery, int playerState, int rsCam, int camX, int camY);

	int GetNewPlayerX() { return newPlayerx; };
	int GetNewPlayerY() { return newPlayery; };
	int GetNewPlayerState(){ return newPlayerState; };
	int GetNewCamXPos() { return camPosX; };
	int GetNewCamYPos() { return camPosY; };
	int IsCamFollowPlayer() { return isCamFollowPlayer; };
	~Gate();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render();

	int GetIdScene() { return gateSwitchSceneId; }
};


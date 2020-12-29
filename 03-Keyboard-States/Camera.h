#pragma once
#include <Windows.h>
#include "global.h"
#define UPPER_LIMIT 737
#define UNDER_LIMIT	1270
class Camera
{
private:
	static Camera* __instance;
	float camx, camy;
	bool isFollowPlayer;
	Camera();
	~Camera();
public:

	static Camera* GetInstance();

	void Update(float xPos, float yPos, EntityType playertype, DWORD dt, int mapWidth, int mapHeight, int nx, int ny, float xPosGo, float xPosBack, float yPosGo, float yPosBack, int &AutoMoveDirection);

	void SetCamPos(float x, float y) { camx = x; camy = y; }
	float GetCamx() { return camx; }
	float GetCamy() { return camy; }
	void SetIsFollowPlayer(bool follow) { isFollowPlayer = follow; };
	bool IsFollowPlayer() { return isFollowPlayer; };
	RECT GetRectCam();
};


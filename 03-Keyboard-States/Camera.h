#pragma once
#include <Windows.h>
#include "global.h"
class Camera
{
	static Camera* __instance;
	float camx, camy;

	Camera();
	~Camera();
public:

	static Camera* GetInstance();

	void Update(float xPos, float yPos, EntityType playertype, DWORD dt, int mapWidth, int mapHeight, int nx, int ny, float xPosGo, float xPosBack, float yPosGo, float yPosBack, int &AutoMoveDirection);

	void SetCamPos(float x, float y) { camx = x; camy = y; }
	float GetCamx() { return camx; }
	float GetCamy() { return camy; }
	RECT GetRectCam();
};


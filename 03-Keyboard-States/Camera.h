#pragma once
#include <Windows.h>

class Camera
{
	static Camera* __instance;
	float camx, camy;

	Camera();
	~Camera();
public:

	static Camera* GetInstance();

	void SetCamPos(float x, float y) { camx = x; camy = y; }
	float GetCamx() { return camx; }
	float GetCamy() { return camy; }
};


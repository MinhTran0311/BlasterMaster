#include "Camera.h"

Camera* Camera::__instance = NULL;

Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

Camera::Camera()
{
	camx = camy = 0;
}

Camera::~Camera() {}

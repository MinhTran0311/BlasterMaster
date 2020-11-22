#include "Camera.h"

Camera* Camera::__instance = NULL;

Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

RECT Camera::GetRectCam()
{
	RECT rect_cam;
	rect_cam.left = camx;
	rect_cam.right = camx + SCREEN_WIDTH;
	rect_cam.top = camy;
	rect_cam.bottom = camy + SCREEN_HEIGHT;
	
	return rect_cam;
}

Camera::Camera()
{
	camx = camy = 0;
}

Camera::~Camera() {}

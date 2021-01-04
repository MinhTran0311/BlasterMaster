#include "Camera.h"
#include "MapManager.h"
Camera* Camera::__instance = NULL;

Camera* Camera::GetInstance()
{
	if (__instance == NULL) __instance = new Camera();
	return __instance;
}

void Camera::Update(float xPos, float yPos, EntityType playertype, DWORD dt, int mapWidth, int mapHeight,int nx, int ny, float xPosGo, float xPosBack,float yPosGo,float yPosBack, int &AutoMoveDirection)
{
	if (playertype==TAG_JASON || playertype==TAG_SMALL_SOPHIA)
	{	
		if (AutoMoveDirection == 0)
		{
			//phương ngang
			if (xPos + SCREEN_WIDTH / 2 >= mapWidth)	// mép phải map
				camx = mapWidth - SCREEN_WIDTH;
			else
			{
				if (xPos < SCREEN_WIDTH / 2)	//ở mép trái map
					camx = 0;
				else
					camx = xPos - SCREEN_WIDTH / 2;
			}
			//phương dọc

			if (yPos + SCREEN_HEIGHT / 2 >= mapHeight)
			{
				camy = mapHeight - SCREEN_HEIGHT;
			}
			else
			{
				if (yPos < SCREEN_HEIGHT / 4)	//ở dưới đáy
					camy = 0;
				else
				{
					if ((yPos - camy) < (SCREEN_HEIGHT / 4))
						camy -= CAMERA_SPEED_UNDERWORLD * dt;
					else if ((yPos - camy) > (SCREEN_HEIGHT / 2))
						camy += CAMERA_SPEED_UNDERWORLD * dt;
				}
			}
			if (yPos - camy > SCREEN_HEIGHT)
				camy = yPos - SCREEN_HEIGHT / 4;
		}
		else if (AutoMoveDirection == 2)
		{
			if (camx < mapWidth)
			{
				//DebugOut(L"cam update 4 %d: \n", MapManager::GetIntance()->GetLeftAlign(MapManager::GetIntance()->GetCurrenStage()));
				//DebugOut(L"Vi tri xcam: %f", camx);
				camx += CAMERA_SPEED_UNDERWORLD * dt;
			}
			else
			{
				isFinishedPassScene = true;
				AutoMoveDirection = 0;
			}
		}
		else if (AutoMoveDirection == 1)
		{
			if (camx > -SCREEN_WIDTH+10)
			{
				camx -= CAMERA_SPEED_UNDERWORLD * dt;
			}
			else {
				AutoMoveDirection = 0;
				isFinishedPassScene = true;
			}
		}
	}
	else if (playertype == TAG_BIG_SOPHIA)
	{
		if (AutoMoveDirection == 1)
		{
			if (nx > 0)
			{
				if (camx < xPosGo)
					camx += CAMERA_SPEED_OVERWORLD * dt;
				else
				{
					camx = xPosGo + 1;
					AutoMoveDirection = 0;
				}
			}
			else if (nx < 0)
			{
				if (camx >= xPosBack)
					camx -= CAMERA_SPEED_OVERWORLD * dt;
				else
				{
					camx = xPosBack - 1;
					AutoMoveDirection = 0;
				}
			}
		}
		else if (AutoMoveDirection == 2)
		{
			if (ny < 0)
			{
				if (camy >= yPosGo)
					camy -= CAMERA_SPEED_OVERWORLD * dt;
				else
				{
					camy = yPosGo - 1;
					AutoMoveDirection = 0;
				}
			}
			else if (ny > 0)
			{
				if (camy <= yPosBack)
					camy += CAMERA_SPEED_OVERWORLD * dt;
				else
				{
					camy = yPosBack + 1;
					AutoMoveDirection = 0;
				}
			}
		}
		else if (isFollowPlayer)
		{
			if (yPos + SCREEN_HEIGHT/2 >= UNDER_LIMIT)
			{
				camy = UNDER_LIMIT - SCREEN_HEIGHT;
			}
			else
			{
				if (yPos < UPPER_LIMIT + SCREEN_HEIGHT / 2)	//ở dưới đáy
					camy = UPPER_LIMIT;
				else
				{
					camy=yPos - SCREEN_HEIGHT / 2;
				}
			}
		}
	}
	this->SetCamPos(camx, camy);
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

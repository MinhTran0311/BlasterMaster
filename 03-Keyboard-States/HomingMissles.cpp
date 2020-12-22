#include "HomingMissles.h"
#include "Grid.h"

HomingMissles::HomingMissles(float xPos, float yPos, int nx)
{
	
	SetState(HOMING_MISSLES_JASON_STATE_FIRE);
	timeDelayed = 0;
	timeDelayedMax = HOMING_MISSLES_DELAY;
	x = xPos;
	y = yPos;
	if (missles.size()!=0)
		missles.clear();
	DebugOut(L"create misles");
	for (int i = 0; i < 3; i++)
	{		
		JasonRocket* singleMissle = new JasonRocket(xPos, yPos, nx, i);
		CGrid::GetInstance()->InsertGrid(singleMissle);
		missles.push_back(singleMissle);
	}
}

HomingMissles::~HomingMissles()
{
}

void HomingMissles::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case HOMING_MISSLES_JASON_STATE_FIRE:
	{
		isActive = true;
		break;
	}
	case HOMING_MISSLES_JASON_STATE_DONE:
	{
		isActive = false;
		break;
	}
	default:
		break;
	}
}

void HomingMissles::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	if (!isActive)
		return;
	
	if (timeDelayed >= timeDelayedMax)
	{
		for (int i = 0; i < missles.size(); i++)
		{
			if (missles.at(i) != nullptr)
				missles.at(i)->setActive(false);
		}
		SetState(HOMING_MISSLES_JASON_STATE_DONE);
		return;
	}
	else
		timeDelayed += dt;
	
	int k = 0;
	for (int i = 0; i < missles.size()-k; i++)
	{
		if (!missles.at(i)->isActiveObject())
		{
			missles.erase(missles.begin() + i);
			k = 1;
			i--;
		}
		else k = 0;
	}
	DebugOut(L"So luong ten lua: %d\n", missles.size());
	//for (int i = 0; i < missles.size(); i++)
	//{
	//	if (missles.at(i) != nullptr && missles.at(i)->isActiveObject())
	//	{
	//		DebugOut(L"loai ten lua %d, %d\n", i,missles.at(i)->isActiveObject());
 //			//missles.at(i)->Update(dt, coObjects);
	//	}
	//}
}

void HomingMissles::Render()
{
	if (!isActive)
		return;

	////DebugOut(L"So luong ten lua %d\n", missles.size());
	//for (int i = 0; i < missles.size(); i++)
	//{
	//	if (missles.at(i) != nullptr && missles.at(i)->isActiveObject())
	//	{
	//		//DebugOut(L"loai ten lua %d\n", missles.at(i)->isActiveObject());
	//	//	missles.at(i)->Render();
	//	}
	//}
}

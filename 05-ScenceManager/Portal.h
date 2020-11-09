#pragma once

#include "Entity.h"

/*
	Object that triggers scene switching
*/
class CPortal : public Entity
{
	int scene_id;	// target scene to switch to 

	int width;
	int height; 
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetSceneId() { return scene_id;  }
};
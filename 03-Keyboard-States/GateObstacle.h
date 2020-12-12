#pragma once
#include "Entity.h"

class GateObstacle : public Entity
{
public:
	GateObstacle();
	~GateObstacle();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects = NULL) {};
	virtual void Render() {};
private:

};

GateObstacle::GateObstacle()
{
}

GateObstacle::~GateObstacle()
{
}
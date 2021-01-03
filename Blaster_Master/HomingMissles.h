#pragma once
#include "Bullet.h"
#include "JasonRocket.h"
#include <vector>
#define HOMING_MISSLES_DELAY	1500

#define HOMING_MISSLES_JASON_STATE_FIRE	0
#define HOMING_MISSLES_JASON_STATE_DONE	100
class HomingMissles : public Bullet
{
public:
	HomingMissles(float xPos, float yPos, int nx);
	~HomingMissles();

	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) { left = x; top = y; right = x; bottom = y; };
	virtual void Update(DWORD dt, vector<LPGAMEENTITY>* coObjects);
	virtual void Render();
	bool CheckExist();
private:
	vector<JasonRocket*> missles;
};


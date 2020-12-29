#pragma once
#include "Animations.h"
#include "Entity.h"
#include "Timer.h"
#define EXPLOSION_ANI	0


#define EXPLOSION_EXIST_TIME	2000
#define EXPLOSION_ANIMATION_SET_ID	201

#define EXPLOSION_WIDTH	23
#define EXPLOSION_HEIGHT	23



class Explosion : public  Entity
{
	//float x, y;
	int width;
	int height;
	DWORD dt;
	int ani;
	LPANIMATION_SET ani_set;
	Timer* timer = new Timer(EXPLOSION_EXIST_TIME);
public:
	void Setposition(float x, float y) { this->x = x; this->y = y; };
	Explosion();
	void Update(DWORD dt);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	~Explosion();
};


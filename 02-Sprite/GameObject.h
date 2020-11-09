#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"


using namespace std;

class Entity
{
	float x; 
	float y;

	float vx;

	int currentState;

	//static vector<LPANIMATION> animations; 
	vector<LPANIMATION> animations;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetState(int state) { this->currentState = state; }

	Entity();

	void Update(DWORD dt);
	void Render();
	~Entity();
};

class Goomba : public Entity
{
	//static vector<LPANIMATION> animations; 
};
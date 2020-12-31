#pragma once
#include "Animations.h"

#include <vector>

#define HEALTH_TYPE_GUN_NULL			0
#define HEALTH_TYPE_UNIT				1
#define HEALTH_TYPE_PLAYER_NULL			2
#define HEALTH_TYPE_CHAR_POW			3
#define HEALTH_TYPE_CHAR_HOV			4
#define HEALTH_TYPE_CHAR_GUN			5

#define HEALTH_SPACE_UNIT				4

#define HEALTH_BAR_HEIGHT				31
#define HEALTH_CHAR_HEIGHT				15
#define HEALTH_BLANK_SPACE				3


class HealthBar
{
private:
	float x, y;
	int currentPlayerHealth, maxPlayerHealth;
	vector<LPANIMATION_SET> health;
	bool isGun;
public:
	HealthBar(int maxHealth, int isGun);
	~HealthBar();
	void Update(int currentHealth, float X, float Y);
	void Render();

	void SetPosition(float X, float Y) { x = X; y = Y; }
	float Getx() { return x; }
	float Gety() { return y; }
	void ReceivePos(float& x, float& y) { x = this->x; y = this->y; }
	void Setx(float X) { this->x = X; }
	void Sety(float Y) { this->y = Y; }
};


#include "Enemy.h"

Enemy::Enemy()
{
	tag = ENEMY;
}

Enemy::~Enemy()
{
}

void Enemy::Render()
{
}

bool Enemy::IsContain(RECT rect1, RECT rect2)
{
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}
	return true;
}

float Enemy::GetDistance(D3DXVECTOR2 pos, D3DXVECTOR2 target)
{
	return (float)sqrt(pow(pos.x - target.x, 2) + pow(pos.y - target.y, 2));
}

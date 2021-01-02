#include "Enemy.h"

Enemy::Enemy()
{
	tag = ENEMY;
	dam = 2;
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
	return (float)sqrt((pos.x - target.x)* (pos.x - target.x) + (pos.y - target.y)* (pos.y - target.y));
}

void Enemy::shootBulletToTarget()
{
}

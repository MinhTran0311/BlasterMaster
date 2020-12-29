#pragma once
#include "Explosion.h"
#include "Game.h"
#include "Animations.h"

Explosion::Explosion()
{
	timer->Start();
	ani_set = CAnimationSets::GetInstance()->Get(EXPLOSION_ANIMATION_SET_ID);
}

void Explosion::Update(DWORD dt)
{

}

void Explosion::Render()
{
	if (!timer->IsTimeUp()) {
		ani_set->at(0)->Render(this->x, this->y, 255);
	}
	else
		Explosion::~Explosion();
}

Explosion::~Explosion()
{
}
void Explosion::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EXPLOSION_WIDTH;
	bottom = y + EXPLOSION_HEIGHT;
}
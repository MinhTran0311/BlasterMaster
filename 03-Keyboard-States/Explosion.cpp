#pragma once
#include "Explosion.h"
#include "Game.h"
#include "Animations.h"

Explosion::Explosion()
{
	DebugOut(L"Tao pho hoa\n");
	timer->Start();
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(EXPLOSION_ANIMATION_SET_ID));
	isActive = true;
}

void Explosion::Update(DWORD dt)
{

}

void Explosion::Render()
{
	DebugOut(L"Phao hoa render 1\n");

	if (isActive)
	{
		animationSet->at(EXPLOSION_ANI)->Render(1, x, y);
	}
	

	DebugOut(L"Phao hoa render\n");
	if (timer->IsTimeUp()) {
		isActive = false;
		/*Explosion::~Explosion();*/
	}
		
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
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "Entity.h"
#include "Sprites.h"

Entity::Entity()
{
	x = y = 0;
	vx = 0.07f;
}

void Entity::Update(DWORD dt)
{
	x += vx*dt;
	if ((vx>0 && x > 290) || (x < 0 && vx<0)) vx=-vx;
}

void Entity::Render()
{
	LPANIMATION ani;
	//if (vx>0) ani = animations[0]; else ani = animations[1];

	if (vx>0) ani = CAnimations::GetInstance()->Get(901); 
	else ani = CAnimations::GetInstance()->Get(900);

	//ani = animations[0];
	ani->Render(x, y);
}

Entity::~Entity()
{
}
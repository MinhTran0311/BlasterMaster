#include <d3dx9.h>
#include <algorithm>

#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "Entity.h"
#include "Sprites.h"


Entity::Entity()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
}

Entity::Entity(float x, float y)
{
	this->SetPosition(x, y);
}

void Entity::Update(DWORD dt)
{
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
	x += dx;
	y += dy;
}

void Entity::Render()
{
}




Entity::~Entity()
{
}
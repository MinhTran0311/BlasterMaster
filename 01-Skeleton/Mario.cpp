#include "Mario.h"

void CMario::Update(DWORD dt)
{
	CGameObject::Update(dt);

	// simple fall down
	vy += MARIO_GRAVITY;
	if (y > 100)
	{
		vy = 0; y = 100.0f;
	}

	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
	if (vx < 0 && x < 0) x = 0;
}

void CMario::Render()
{
	int ani;
	if (color_Type == 0)
	{
		if (vx == 0)
		{
			if (nx > 0) ani = MARIO_ANI_IDLE_RIGHT;
			else ani = MARIO_ANI_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_WALKING_RIGHT;
		else ani = MARIO_ANI_WALKING_LEFT;
	}
	else
	{
		if (vx == 0)
		{
			if (nx > 0) ani = MARIO_ANI_IDLE_RIGHT_YELLOW;
			else ani = MARIO_ANI_IDLE_LEFT_YELLOW;
		}
		else if (vx > 0)
			ani = MARIO_ANI_WALKING_RIGHT_YELLOW;
		else ani = MARIO_ANI_WALKING_LEFT_YELLOW;
	}

	animations[ani]->Render(x, y);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		color_Type = 0;
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		color_Type = 0;
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		color_Type = 0;
		if (y == 100)
			vy = -MARIO_JUMP_SPEED_Y;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
		//colored
	case MARIO_STATE_WALKING_RIGHT_YELLOW:
		color_Type = 1;
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT_YELLOW:
		color_Type = 1;
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_JUMP_YELLOW:
		color_Type = 1;
		if (y == 100)
			vy = -MARIO_JUMP_SPEED_Y;
	case MARIO_STATE_IDLE_YELLOW:
		vx = 0;
		break;

	}
}


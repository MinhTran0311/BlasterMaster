#include "Jason.h"

JASON::JASON(float x, float y)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_PLAYER));
	SetState(SOPHIA_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	current_Jumpy = 0;
	isDeath = false;
	alpha = 255;
}

JASON* JASON::instance = NULL;
JASON* JASON::GetInstance()
{
	if (instance == NULL)
		instance = new JASON();
	return instance;
}

void JASON::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case SOPHIA_STATE_WALKING_RIGHT:
		vx = SOPHIA_WALKING_SPEED;
		nx = 1;
		break;
	case SOPHIA_STATE_WALKING_LEFT:
		vx = -SOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	case SOPHIA_STATE_JUMP:
		isPressJump = true;
		if (isJumping == true)
			return;
		else
		{
			isJumpHandle = false;
			isJumping = true;
			vy = -SOPHIA_JUMP_SPEED_Y;		
			current_Jumpy = y;
		}
		break;
	case SOPHIA_STATE_IDLE:
		isPressJump = false;
		//isJumpHandle = true;
		if (vx > 0) {
			vx -= SOPHIA_WALKING_ACC*dt;
			if (vx < 0)
				vx = 0;
		}
		else if (vx < 0)
		{
			vx += SOPHIA_WALKING_ACC*dt;
			if (vx > 0)
				vx = 0;
		}
		break;
	case SOPHIA_STATE_OUT:
		vx = 0;
		isEjecting = true;
		break;
	}
}

void JASON::Update(DWORD dt, vector<LPGAMEENTITY>* colliable_objects, vector<LPGAMEENTITY>* coEnemies)
{
	if (isDoneDeath)
		return;
	
	//health update
	Entity::Update(dt);
	//fall down
#pragma region fall down 
	vy += SOPHIA_GRAVITY * dt;
	//check player's height
	if (isJumping && current_Jumpy - y >= HEIGHT_LEVER1 && isJumpHandle == false)
	{
		if (!isPressJump) 
			vy = 0;
		isJumpHandle = true;
}
#pragma endregion
#pragma region gun flip
	if (isPressFlipGun == false)
	{
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_1)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_2)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_3)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_4)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_1)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_2)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_3)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_4)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_RIGHT)->ResetCurrentFrame();
		animationSet->at(SOPHIA_ANI_GUN_FLIP_LEFT)->ResetCurrentFrame();
	}
#pragma endregion
	if (this->y >= 146)
	{
		isJumping = false;
		y = 146;
	}
}

void JASON::Render()
{
	if (isDoneDeath)
		return;
	//RenderBoundingBox();

	int ani = -1;
	int current_frame;
	alpha = 255;

	if (isDeath)
	{
		ani = SOPHIA_JASON_ANI_DIE;
		animationSet->at(ani)->Render(nx, x - DURATION_X_TO_DIE, y - DURATION_Y_TO_DIE, alpha);
		if (animationSet->at(ani)->GetFrame() > 1)
		{
			isDoneDeath = true;
		}
	}
	else if (isEjecting)
	{
		ani = SOPHIA_JASON_ANI_EJECTING;
		animationSet->at(ani)->Render(nx, x, y-8, alpha);
		if (animationSet->at(ani)->GetFrame() >= 1)
		{
			isEjecting = false;
		}
	}
	else if (isPressFlipGun == true)
	{
		if (vx == 0)
		{
			if (nx > 0)
			{
				//idle theo walking
				current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrameStopWalking();
				switch (current_frame)
				{
				case SOPHIA_STOP_WALKING_SPRITE2:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_2;
					break;
				case SOPHIA_STOP_WALKING_SPRITE3:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_3;
					break;
				case SOPHIA_STOP_WALKING_SPRITE4:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_4;
					break;
				default:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_RIGHT_1;
					break;
				}
			}
			else
			{
				current_frame = animationSet->at(SOPHIA_ANI_JASON_WALKING_LEFT)->GetFrameStopWalking();
				switch (current_frame)
				{
				case SOPHIA_STOP_WALKING_SPRITE2:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_2;
					break;
				case SOPHIA_STOP_WALKING_SPRITE3:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_3;
					break;
				case SOPHIA_STOP_WALKING_SPRITE4:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_4;
					break;
				default:
					ani = SOPHIA_ANI_GUN_FLIP_IDLE_LEFT_1;
					break;
				}
			}

			if (isGunFlipping == false)
			{
				animationSet->at(ani)->OldRender(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
				if (animationSet->at(ani)->GetFrame() == 1)
				{
					//DebugOut(L"[frame]: %d;\n", animation_set->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrame());
					isGunFlipping = true;
				}
				return;

			}
			else
			{
				animationSet->at(ani)->RenderFrame(1, x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
				return;
			}
		}
		else if (vx > 0)
			ani = SOPHIA_ANI_GUN_FLIP_RIGHT;
		else
			ani = SOPHIA_ANI_GUN_FLIP_LEFT;

		if (isGunFlipping == false)
		{
			animationSet->at(ani)->RenderGunFlip(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
			if (animationSet->at(ani)->GetFrame() > 3)
			{
				//DebugOut(L"[frame]: %d;\n", animation_set->at(SOPHIA_ANI_JASON_WALKING_RIGHT)->GetFrame());
				isGunFlipping = true;
			}
			return;
		}
		else
		{
			animationSet->at(ani)->RenderGunFlipTargetTop(x, y - SOPHIA_JASON_HEIGHT_GUN_FLIP, alpha);
			return;
		}
	}
	else
	{
		if (isJumping == false)
		{
			if (vx == 0)
			{
				if (nx > 0)
				{
					ani = SOPHIA_ANI_JASON_WALKING_RIGHT;
					current_frame = animationSet->at(ani)->GetFrameStopWalking();
				}
				else
				{
					ani = SOPHIA_ANI_JASON_WALKING_LEFT;
					current_frame = animationSet->at(ani)->GetFrameStopWalking();
				}
				animationSet->at(ani)->RenderFrame(current_frame, x, y, alpha);
				isGunFlipping = false;
				return;
			}
			else if (vx > 0)
				ani = SOPHIA_ANI_JASON_WALKING_RIGHT;
			else ani = SOPHIA_ANI_JASON_WALKING_LEFT;
			animationSet->at(ani)->OldRender(x, y, alpha);
			isGunFlipping = false;
		}
		else {
			if (vx == 0 && vy > 0)
			{
				if (nx > 0) ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_DOWN_IDLE_LEFT;
			}
			else if (vx == 0 && vy <= 0)
			{
				if (nx > 0) ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_UP_IDLE_LEFT;
			}
			else if (vy > 0)
			{
				if (vx > 0) ani = SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_DOWN_WALKING_LEFT;
			}
			else if (vy <= 0)
			{
				if (vx > 0) ani = SOPHIA_ANI_JASON_JUMP_UP_WALKING_RIGHT;
				else ani = SOPHIA_ANI_JASON_JUMP_UP_WALKING_LEFT;
			}
			isGunFlipping = false;
			animationSet->at(ani)->OldRender(x, y, alpha);
			return;
		}
	}

}

void JASON::Reset()
{
	//health = MAX_HEALTH;
	isDoneDeath = false;
	isDeath = false;
	SetState(SOPHIA_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

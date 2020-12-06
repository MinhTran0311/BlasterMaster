#include "Small_Sophia.h"

Small_Sophia::Small_Sophia(float x, float y, int _health, int _gundam) : Player()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_SMALL_SOPHIA));
	untouchable = 0;
	SetState(SMALL_SOPHIA_STATE_IDLE);
	_PlayerType = EntityType::TAG_SMALL_SOPHIA;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	backup_JumpY = 0;
	dam = _gundam;
	health = _health;
	isImmortaling = false;
	canFire = true;
	isCrawl = false;
	alpha = 255;
}
Small_Sophia* Small_Sophia::instance = NULL;
Small_Sophia* Small_Sophia::GetInstance()
{
	if (instance == NULL)
		instance = new Small_Sophia();
	return instance;
}

void Small_Sophia::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Player::Update(dt, coObjects);
	//if (isDoneDeath)
	//	return;
	//if (health <= 0)
	//{
	//	isDeath = true;
	//	vx = 0;
	//	vy = 0;
	//}
	//Entity::Update(dt);
#pragma region fall down 
	vy += SOPHIA_GRAVITY * dt;
	//check player's height
	//if (isJumping && current_Jumpy - y >= HEIGHT_LEVER1 && isJumpHandle == false)
	//{
	//	if (!isPressJump)
	//		vy = 0;
	//	isJumpHandle = true;
	//}
#pragma endregion

#pragma region Timer
	if (isImmortaling && immortalTimer->IsTimeUp())
	{
		isImmortaling = false;
		immortalTimer->Reset();
	}
#pragma endregion
	if (!canFire && FireTimer->IsTimeUp())
	{
		canFire = true;
		FireTimer->Reset();
	}

	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK || coObjects->at(i)->GetType() == EntityType::TAG_GATE)
			colliable_Objects->push_back(coObjects->at(i));
	}
#pragma region Collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	if (state != SMALL_SOPHIA_STATE_DIE)
		CalcPotentialCollisions(colliable_Objects, coEvents);

	//ABBA with objects
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (this->IsCollidingObject(coObjects->at(i)) && (coObjects->at(i)->GetType() == EntityType::ENEMY))
		{
			Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));
			//re check
			if (isJumping)
			{
				this->SetState(SMALL_SOPHIA_STATE_IDLE);
				isJumping = false;
				isJumpHandle = true;
			}
			SetInjured(enemy->GetDamage());
		}
	}


	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->obj->GetType() == EntityType::TAG_BRICK)
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				if (e->ny != 0)
				{

					if (e->ny != 0)
					{
						vy = 0;
						if (ny < 0)
							isJumping = false;
					}

					if (e->nx != 0)
						vx = 0;
				}
			}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

//void Small_Sophia::SetInjured(int dame)
//{
//	if (isImmortaling)
//		return;
//	health -= dame;
//	dam -= dame;
//
//	StartUntouchable();
//	immortalTimer->Start();
//	isImmortaling = true;
//}

void Small_Sophia::Render()
{
	if (isDoneDeath)
		return;
	//RenderBoundingBox();

#pragma region Khai báo biến
	int ani = -1;
	int current_frame; //luu frame khi dang di chuyen ma dung lai
	alpha = 255;
	if (isImmortaling) alpha = 128;
#pragma endregion

	if (isDeath)
	{
		ani = SOPHIA_ANI_SMALL_DIE;
		animationSet->at(ani)->Render(nx, x, y, alpha);
		if (animationSet->at(ani)->GetFrame() > LAST_FRAME_DIE)
			isDoneDeath = true;
	}
	else
	{
		if (isJumping)
		{
			ani = SOPHIA_ANI_SMALL_JUMP;
		}
		else if (isCrawl)
		{
			if (vx == 0)
				ani = SOPHIA_ANI_SMALL_IDLE_CRAWL_RIGHT;
			else
				ani = SOPHIA_ANI_SMALL_WALKING_CRAWL_RIGHT;
		}
		else
		{
			if (vx == 0)
				ani = SOPHIA_ANI_SMALL_IDLE_RIGHT;
			else
				ani = SOPHIA_ANI_SMALL_WALKING_RIGHT;
		}
	}
	animationSet->at(ani)->Render(nx, x, y, alpha);
}

void Small_Sophia::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case SMALL_SOPHIA_STATE_WALKING_RIGHT:
		if (isCrawl)
			vx = SMALL_SOPHIA_CRAWLING_SPEED;
		else
			vx = SMALL_SOPHIA_WALKING_SPEED;
		nx = 1;
		break;
	case SMALL_SOPHIA_STATE_WALKING_LEFT:
		if (isCrawl)
			vx = -SMALL_SOPHIA_CRAWLING_SPEED;
		else
			vx = -SMALL_SOPHIA_WALKING_SPEED;
		nx = -1;
		break;
	case SMALL_SOPHIA_STATE_CRAWL:
		if (!isJumping)
		{
			if (isCrawl)
			{
				y -= (SMALL_SOPHIA_BBOX_HEIGHT - SMALL_SOPHIA_CRAWL_BBOX_HEIGHT);
				vx = 0;
				isCrawl = false;
			}
			else
				isCrawl = true;
		}
		break;
	case SMALL_SOPHIA_STATE_JUMP:
		isPressJump = true;
		if (isCrawl)
			return;

		if (isJumping)
			return;
		else
		{
			isJumpHandle = false;
			isJumping = true;
			vy = -SMALL_SOPHIA_JUMP_SPEED_Y;
		}
		break;
	case SMALL_SOPHIA_STATE_IDLE:
		isPressJump = false;
		if (!isCrawl)
		{
			if (vx > 0) {
				vx -= SMALL_SOPHIA_WALKING_ACC * dt;
				if (vx < 0)
					vx = 0;
			}
			else if (vx < 0) {
				vx += SMALL_SOPHIA_WALKING_ACC * dt;
				if (vx > 0)
					vx = 0;
			}
		}
		break;
	case SMALL_SOPHIA_STATE_CRAWL_STOP:
		if (isCrawl)
			vx = 0;
		break;
	case SMALL_SOPHIA_STATE_DIE:
	case SMALL_SOPHIA_STATE_IN:
	case SMALL_SOPHIA_STATE_OUT:
		isCrawl = false;
		if (nx > 0) {
			vx -= 3 * SMALL_SOPHIA_WALKING_ACC * dt;
			if (vx < 0)
				vx = 0;
		}
		else if (nx < 0) {
			vx += 3 * SMALL_SOPHIA_WALKING_ACC * dt;
			if (vx > 0)
				vx = 0;
		}
		vy = -SMALL_SOPHIA_JUMP_SPEED_Y / 1.5f;
		break;
	}
}

void Small_Sophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isDoneDeath)
	{
		if (isCrawl)
		{
			left = x;
			top = y;
			right = x + SMALL_SOPHIA_CRAWL_BBOX_WIDTH;
			bottom = y + SMALL_SOPHIA_CRAWL_BBOX_HEIGHT;
		}
		else
		{
			left = x;
			top = y;
			right = x + SMALL_SOPHIA_BBOX_WIDTH;
			bottom = y + SMALL_SOPHIA_BBOX_HEIGHT;
		}
	}
}

void Small_Sophia::FireBullet(int mode)
{
	if (!canFire)
		return;
	if (CGrid::GetInstance()->CheckBulletLimitation(SMALL_SOPHIA_NORMAL_BULLET, this->Getx(), this->Gety(), 3))
	{
		Bullet* bullet = new SmallSophiaBullet(this->Getx(), this->Gety(), 0, nx);
		CGrid::GetInstance()->InsertGrid(bullet);
	}
	FireTimer->Start();
	canFire = false;
}

void Small_Sophia::Reset()
{
	SetState(SMALL_SOPHIA_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

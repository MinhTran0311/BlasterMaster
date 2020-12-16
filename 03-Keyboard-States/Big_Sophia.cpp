#include "Big_Sophia.h"
#include "Item.h"
#include "PowerUp.h"
#include "GunUp.h"
#include "InjuringBrick.h"

Big_Sophia::Big_Sophia(float x, float y, int _health, int _gundam)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_SOPHIA));
	this->SetState(BIG_SOPHIA_STATE_IDLE);
	_PlayerType = TAG_BIG_SOPHIA;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	backup_x = 0;
	backup_y = 0;
	dam = _gundam;
	health = _health;
	isVerticalMove = false;
	_isAutoRun = false;
}

Big_Sophia::~Big_Sophia()
{
}

void Big_Sophia::AutoRun(int direction)
{
	if (!_isAutoRun)
	{
		_isAutoRun = true;
		backup_x = this->x;
		backup_y = this->y;
		directionAutoRun = direction;
	}
}

void Big_Sophia::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	DebugOut(L"nx: %d, ny: %d\n", nx, ny);
	Player::Update(dt, coObjects);
#pragma region Timer
	//if (isImmortaling && immortalTimer->IsTimeUp())
	//{
	//	isImmortaling = false;
	//	immortalTimer->Reset();
	//}
	if (_isAutoRun == true)
	{
		if (directionAutoRun == 1 && abs(x - backup_x) <= GATE_HORIZONTAL_LONG)
		{
			if (nx != 0)
			{
				vx = 0.05f * nx;
				vy = 0;
			}
			//DebugOut(L"Sai lech x: %f\n", abs(x - backup_x));
		}
		else if (directionAutoRun == 2 && abs(y - backup_y) <= GATE_VERTICAL_LONG)
		{
			if (ny != 0)
			{
				vy = 0.07f * ny;
				vx = 0;
			}
			//DebugOut(L"Sai lech y: %f\n", abs(y - backup_y));
		}
		else
		{
			_isAutoRun = false;
			GateColliding = false;
		}
		Entity::Update(dt, coObjects);
		x += dx;
		y += dy;
		return;
	}
#pragma endregion
#pragma region Xử lý va chạm
	CollisionHandle(coObjects);
	////ABBA with objects
	//vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();
	//bool isInjured = false;
	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	//if (this->IsCollidingObject(coObjects->at(i)))
	//	//{
	//	//	switch (coObjects->at(i)->GetType())
	//	//	{
	//	//	case ENEMY:
	//	//	{
	//	//		Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));
	//	//		//re check
	//	//		SetInjured(enemy->GetDamage());
	//	//		break;
	//	//	}
	//	//	case ITEM:
	//	//	{
	//	//		LPGAMEITEM item = dynamic_cast<LPGAMEITEM>(coObjects->at(i));
	//	//		if (item->getItemType() == EntityType::TAG_ITEM_POWER_UP)
	//	//		{
	//	//			if (this->GetHealth() + ITEM_POWER_UP_RESTORE <= MAX_HEALTH)
	//	//				this->AddHealth(ITEM_POWER_UP_RESTORE);
	//	//			else
	//	//				this->SetHealth(MAX_HEALTH);
	//	//		}
	//	//		else if (item->getItemType() == EntityType::TAG_ITEM_GUN_UP)
	//	//		{
	//	//			if (this->GetgunDam() + ITEM_GUN_UP_RESTORE <= MAX_GUNDAM)
	//	//				this->AddgunDam(ITEM_GUN_UP_RESTORE);
	//	//			else
	//	//				this->SetgunDam(MAX_GUNDAM);
	//	//		}
	//	//		item->setActive(false);
	//	//		break;
	//	//	}
	//	//	case TAG_INJURING_BRICK:
	//	//	{
	//	//		InjuringBrick* injuringBricks = dynamic_cast<InjuringBrick*>(coObjects->at(i));
	//	//		SetInjured(injuringBricks->GetDamage());
	//	//		break;
	//	//	}
	//	//	//case TAG_GATE:
	//	//	//{
	//	//	//	gate = coObjects->at(i);
	//	//	//	DebugOut(L"big sophia dung tuong\n");
	//	//	//	GateColliding = true;
	//	//	//	break;
	//	//	//}
	//	//	}
	//	//}
	//	CollideWithObject(coObjects->at(i), isInjured);
	//	if (coObjects->at(i)->GetType() == TAG_BRICK || coObjects->at(i)->GetType() == TAG_GATE_OVERWORLD || coObjects->at(i)->GetType() == TAG_SOFT_BRICK || coObjects->at(i)->GetType() == TAG_GATE)
	//		colliable_Objects->push_back(coObjects->at(i));
	//}
	//if (!isInjured)
	//	alpha = 255;
	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;
	//coEvents.clear();
	//// turn off collision when die
	//if (state != BIG_SOPHIA_STATE_DIE)
	//	CalcPotentialCollisions(colliable_Objects, coEvents);
	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];
	//		if (e->obj->GetType() == EntityType::TAG_BRICK || e->obj->GetType() == EntityType::TAG_SOFT_BRICK)
	//		{
	//			x += min_tx * dx + nx * 0.4f;
	//			y += min_ty * dy + ny * 0.4f;
	//			if (e->ny != 0)
	//			{
	//				if (e->ny != 0)
	//				{
	//					vy = 0;
	//				}
	//				if (e->nx != 0)
	//					vx = 0;
	//			}
	//		}
	//		else if (e->obj->GetType() == EntityType::TAG_GATE_OVERWORLD || e->obj->GetType() == EntityType::TAG_GATE)
	//		{
	//			gate = e->obj;
	//			DebugOut(L"big sophia dung tuong\n");
	//			GateColliding = true;
	//		}
	//	}
	//}
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void Big_Sophia::Render()
{
	RenderBoundingBox();
	int ani = -1;
	int current_frame;
	if (isDeath)
	{
		ani = BIG_SOPHIA_ANI_DIE;
		animationSet->at(ani)->Render(nx, x, y, alpha);
		if (animationSet->at(ani)->GetFrame() > LAST_FRAME_BIG_SOPHIA_DIE)
			isDoneDeath = true;
	}
	else if (_isAutoRun)
		return;
	else
	{
		if (vx == 0 && vy == 0)
		{
			if (!isVerticalMove)
			{
				if (nx > 0)
					ani = BIG_SOPHIA_ANI_IDLE_RIGHT;
				else if (nx < 0)
					ani = BIG_SOPHIA_ANI_IDLE_LEFT;
			}
			else
			{
				if (ny > 0)
					ani = BIG_SOPHIA_ANI_IDLE_BOTTOM;
				else if (ny < 0)
					ani = BIG_SOPHIA_ANI_IDLE_TOP;
			}
		}
		else
		{
			if (vx > 0)
				ani = BIG_SOPHIA_ANI_WALKING_RIGHT;
			else if (vx < 0)
				ani = BIG_SOPHIA_ANI_WALKING_LEFT;
			else if (vy > 0)
				ani = BIG_SOPHIA_ANI_WALKING_BOTTOM;
			else if (vy < 0)
				ani = BIG_SOPHIA_ANI_WALKING_TOP;
		}
		animationSet->at(ani)->Render(1, x, y,alpha);

	}
}

void Big_Sophia::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BIG_SOPHIA_BBOX_WIDTH;
	bottom = y + BIG_SOPHIA_BBOX_HEIGHT;
}

void Big_Sophia::SetState(int state)
{
	Entity::SetState(state);

	switch (state)
	{
	case BIG_SOPHIA_STATE_IDLE:
	{
		vx = 0;
		vy = 0;
		break;
	}
	case BIG_SOPHIA_STATE_WALKING_RIGHT:
	{
		ny = 0;
		nx = 1;
		vx = BIG_SOPHIA_WALKING_SPEED;
		isVerticalMove = false;
		break;
	}
	case BIG_SOPHIA_STATE_WALKING_LEFT:
	{
		ny = 0;
		nx = -1;
		vx = -BIG_SOPHIA_WALKING_SPEED;
		isVerticalMove = false;

		break;
	}
	case BIG_SOPHIA_STATE_WALKING_TOP:
	{
		nx = 0;
		ny = -1;
		vy = -BIG_SOPHIA_WALKING_SPEED;
		isVerticalMove = true;
		break;
	}
	case BIG_SOPHIA_STATE_WALKING_BOTTOM:
	{
		nx = 0;
		ny = 1;
		vy = BIG_SOPHIA_WALKING_SPEED;
		isVerticalMove = true;
		break;
	}
	case BIG_SOPHIA_STATE_DIE:
	{
		vx = 0;
		vy = 0;
		break;
	}
	}
}
void Big_Sophia::FireBullet(int mode)
{
	DebugOut(L"dan big tao duoc");
	if (!canFire)
		return;
	Bullet* bullet = new BigSophiaBullet(this->Getx(), this->Gety(), dam, nx, ny);
	CGrid::GetInstance()->InsertGrid(bullet);
	FireTimer->Start();
	canFire = false;
}
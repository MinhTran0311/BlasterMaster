#include "Big_Sophia.h"
#include "Item.h"
#include "PowerUp.h"
#include "GunUp.h"
#include "InjuringBrick.h"

Big_Sophia::Big_Sophia(float x, float y, int _health, int _gundam)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BIG_SOPHIA));
	_PlayerType = TAG_BIG_SOPHIA; 
	//untouchable = 0;
	this->SetState(BIG_SOPHIA_STATE_IDLE);
	this->x = x;
	this->y = y;
	start_x = this->x = x;
	start_y = this->y = y;
	dam = _gundam;
	health = _health;
	isImmortaling = false;
	isDeath = false;
}

Big_Sophia::~Big_Sophia()
{
}

void Big_Sophia::AutoRun(int direction)
{
	if (!_isAutoRun)
	{
		_isAutoRun = true;
		backup_x = x;
		backup_y = y;
		directionAutoRun = direction;
	}
}

void Big_Sophia::FireBullet(int type)
{
}

void Big_Sophia::Reset()
{
	SetState(BIG_SOPHIA_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void Big_Sophia::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{

#pragma region Timer
	if (isImmortaling && immortalTimer->IsTimeUp())
	{
		isImmortaling = false;
		immortalTimer->Reset();
	}
	if (_isAutoRun)
	{
		if (directionAutoRun == 1 && abs(x - backup_x) <= 82)
		{
			if (nx != 0)
			{
				vx = 0.05f * nx;
				vy = 0.0f;
			}
		}
		else if (directionAutoRun == 2 && abs(y - backup_y) <= 125)
		{
			if (ny != 0)
			{
				vy = 0.07f * ny;
				vx = 0.0f;
			}
		}
		else
		{
			_isAutoRun = false;
		}
		Player::Update(dt, coObjects);
		x += dx;
		y += dy;
		return;
	}
#pragma endregion
	Player::Update(dt, coObjects);
#pragma region Xử lý va chạm
	//ABBA with objects
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (this->IsCollidingObject(coObjects->at(i)))
		{
			switch (coObjects->at(i)->GetType())
			{

			case EntityType::ENEMY:
			{
				Enemy* enemy = dynamic_cast<Enemy*>(coObjects->at(i));
				//re check
				SetInjured(enemy->GetDamage());
				break;
			}
			case EntityType::ITEM:
			{
				LPGAMEITEM item = dynamic_cast<LPGAMEITEM>(coObjects->at(i));
				if (item->getItemType() == EntityType::TAG_ITEM_POWER_UP)
				{
					if (this->GetHealth() + ITEM_POWER_UP_RESTORE <= MAX_HEALTH)
						this->AddHealth(ITEM_POWER_UP_RESTORE);
					else
						this->SetHealth(MAX_HEALTH);
				}
				else if (item->getItemType() == EntityType::TAG_ITEM_GUN_UP)
				{
					if (this->GetgunDam() + ITEM_GUN_UP_RESTORE <= MAX_GUNDAM)
						this->AddgunDam(ITEM_GUN_UP_RESTORE);
					else
						this->SetgunDam(MAX_GUNDAM);
				}
				item->setActive(false);
				break;
			}
			case EntityType::TAG_INJURING_BRICK:
			{
				InjuringBrick* injuringBricks = dynamic_cast<InjuringBrick*>(coObjects->at(i));
				SetInjured(injuringBricks->GetDamage());
				break;
			}
			}
		}
	}
	vector<LPGAMEENTITY>* colliable_Objects = new vector<LPGAMEENTITY>();

	for (int i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK || coObjects->at(i)->GetType() == EntityType::TAG_GATE_OVERWORLD)
			colliable_Objects->push_back(coObjects->at(i));
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	// turn off collision when die 
	if (state != BIG_SOPHIA_STATE_DIE)
		CalcPotentialCollisions(colliable_Objects, coEvents);

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
					}

					if (e->nx != 0)
						vx = 0;
				}
			}
			else if (e->obj->GetType() == EntityType::TAG_GATE_OVERWORLD)
			{
				gate = dynamic_cast<GateOverworld*>(e->obj);
				DebugOut(L"big sophia dung tuong loai 2");
				GateColliding = true;
			}

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
	else
	{
		if (vx==0 && vy==0)
		{
			if (nx > 0)
				ani = BIG_SOPHIA_ANI_IDLE_RIGHT;
			else if (nx < 0)
				ani = BIG_SOPHIA_ANI_IDLE_LEFT;
			else if (ny > 0)
				ani = BIG_SOPHIA_ANI_IDLE_BOTTOM;
			else if (ny < 0)
				ani = BIG_SOPHIA_ANI_IDLE_TOP;
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
		animationSet->at(ani)->Render(nx, x, y);
		
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
		vx = 0;
		vy = 0;
		break;
	case BIG_SOPHIA_STATE_WALKING_RIGHT:
		nx = 1;
		vx = BIG_SOPHIA_WALKING_SPEED;
		break;
	case BIG_SOPHIA_STATE_WALKING_LEFT:
		nx = -1;
		vx = -BIG_SOPHIA_WALKING_SPEED;
		break;
	case BIG_SOPHIA_STATE_WALKING_TOP:
		ny = -1;
		vy = -BIG_SOPHIA_WALKING_SPEED;
		break;
	case BIG_SOPHIA_STATE_WALKING_BOTTOM:
		ny = 1;
		vy = BIG_SOPHIA_WALKING_SPEED;
		break;
	case BIG_SOPHIA_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	}
}

#include "BossBullet.h"
#include "Player.h"


BossBullet::BossBullet(float xPos, float yPos, LPGAMEENTITY t)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BOSS_ENEMY_BULLET));
	tag = EntityType::BULLET;
	isHitEnemy = false;
	dam = 1; 
	this->SetState(BOSS_ENEMY_BULLET_STATE_FLYING);
	typeBullet = BOSS_BULLET;
	bullet_speed = BOSS_BULLET_SPEED;
	x = xPos;
	y = yPos;
	if (t->Getx() > x) nx = 1;
	else if (t->Getx() < x) nx = -1;
	else nx = 0;

	ny = 1;
	timeDelayed = 0;
	timeDelayedMax = BOSS_ENEMY_DELAY;
	target = t;
	x_Start_Pos = xPos;
	y_Start_Pos = yPos;
	xTarget = target->Getx();
	yTarget = target->Gety();
	isActive = true;
}

BossBullet::~BossBullet()
{
}

void BossBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BOSS_ENEMY_BULLET_BBOX_WIDTH;
	bottom = top + BOSS_ENEMY_BULLET_BBOX_HEIGHT;
}

void BossBullet::Update(DWORD dt, vector<LPGAMEENTITY>* co_Objects)
{
	if (!isActive || state != BOSS_ENEMY_BULLET_STATE_FLYING)
	{
		return;
	}
	if (timeDelayed >= timeDelayedMax)
	{
		isActive = false;
		timeDelayed = 0;
		return;
	}
	else
	{
		timeDelayed += dt;
		CalVelocity(vx, vy, target);
	}
	Entity::Update(dt);

	if (this->IsCollidingObject(target))
	{
		DebugOut(L"hit player");
		dynamic_cast<Player*>(target)->EnemyBulletHitPlayer(dam);
		isActive = false;
		this->SetState(BOSS_ENEMY_BULLET_STATE_HIT_PLAYER);
		return;
	}
	else
	{
		x += dx;
		y += dy;
	}

}

void BossBullet::Render()
{
	if (!isActive)
		return;
	int ani;
	if (state == BOSS_ENEMY_BULLET_STATE_FLYING)
	{
		ani = BOSS_ENEMY_BULLET_STATE_FLYING;
		animationSet->at(ani)->Render(1, x, y);
	}
	else isActive = false;
}

void BossBullet::SetState(int state)
{
	Bullet::SetState(state);
}

void BossBullet::CalVelocity(float& vx, float& vy, LPGAMEENTITY t)
{
	float d = sqrt((x_Start_Pos - xTarget) * (x_Start_Pos - xTarget) + (y_Start_Pos - yTarget) * (y_Start_Pos - yTarget));
	vx = ((xTarget - x_Start_Pos) / d) * BOSS_BULLET_SPEED;
	vy = ((yTarget - y_Start_Pos) / d) * BOSS_BULLET_SPEED;
}

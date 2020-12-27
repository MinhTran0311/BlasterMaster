#pragma once
#include "Boss.h"
#include "Utils.h"
#include "Game.h"
//#include "PlayScence.h"
//#include "EnemyBullet.h"
#include "Vec2.h"

CBoss::CBoss(float xPos, float yPos, LPGAMEENTITY t) :
	BigClawLeft(ANIMATION_SET_BOSS_CLAW_LEFT),
	BigClawRight(ANIMATION_SET_BOSS_CLAW_RIGHT)

{
	x = xPos;
	y = yPos;
	startX = xPos;
	startY = yPos;

	target = t;

	isActive = true;
	dam = 1;
	health = 30;
	nx = -1;
	SetState(BOSS_STATE_WALKING_LEFT);
	vx = -BOSS_WALKING_SPEED;
	vy = BOSS_WALKING_SPEED;
	Init();
	//for (int i = 0; i < 4; i++)
	//{
	//	int a = 400 * i;
	//	Timer* timerElement = new Timer(BOSS_BULLET_SHOOT_DELAY, a);
	//	timerElement->Start();
	//	timer.push_back(timerElement);
	//}
	bursttimer->Start();
}

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	this->isBoss = true;
	left = x;
	top = y;
	right = x + BOSS_BBOX_WIDTH;

	if (state == BOSS_STATE_DIE)
		bottom = y + BOSS_BBOX_HEIGHT_DIE;
	else
		bottom = y + BOSS_BBOX_HEIGHT;
}

void CBoss::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{
	Entity::Update(dt, coObjects);

	if (health <= 0)
		this->isActive = false;
	//
	// TO-DO: make sure Boss can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
	//DebugOut(L"x: %f, y: %f", x, y);

	if (vx < 0 && x < (startX - 100)) {
		x = startX - 100;
		SetState(BOSS_STATE_WALKING_RIGHT);
		nx = 1;
		vx = BOSS_WALKING_SPEED;
	}
	if (vx > 0 && x > startX) {
		x = startX; vx = -vx;
		nx = -1;
		vx = -BOSS_WALKING_SPEED;
		SetState(BOSS_STATE_WALKING_LEFT);
	}
	if (vy < 0 && y < (startY - 25)) {
		y = startY - 25;
		vy = BOSS_WALKING_SPEED;
	}
	if (vy > 0 && y > startY) {
		y = startY;
		vy = -BOSS_WALKING_SPEED;
	}

	updateTarget1();
	updateTarget2();
	this->BigClawLeft.Follow(Target1);


	LeftArm[3].Follow(BigClawLeft);
	LeftArm[3].calculateEndpoint();
	for (int i = 2; i >= 0; i--)
	{
		LeftArm[i].Follow(LeftArm[i + 1]);
		this->LeftArm[i].calculateEndpoint();
	}

	//Rebasing the arm to the boss
	this->LeftArm[0].setStartPoint(Vec2(x, y + 20));
	this->LeftArm[0].calculateEndpoint();
	for (int i = 1; i < 4; i++)
	{
		this->LeftArm[i].setStartPoint(this->LeftArm[i - 1].getEndpoint());
		this->LeftArm[i].calculateEndpoint();

	}
	this->BigClawLeft.setStartPoint(LeftArm[3].getEndpoint());
	this->BigClawLeft.calculateEndpoint();
	//DebugOut(L"Claw end x, %d ", this->BigClawLeft.getEndpoint().x);
	//DebugOut(L"Claw end y, %d ", this->BigClawLeft.getEndpoint().y);

	this->BigClawRight.Follow(Target2);
	RightArm[3].Follow(BigClawRight);
	RightArm[3].calculateEndpoint();
	for (int i = 2; i >= 0; i--)
	{
		RightArm[i].Follow(RightArm[i + 1]);
		this->RightArm[i].calculateEndpoint();
	}

	//Rebasing the arm to the boss
	this->RightArm[0].setStartPoint(Vec2(x + 40, y + 20));
	this->RightArm[0].calculateEndpoint();
	for (int i = 1; i < 4; i++)
	{
		this->RightArm[i].setStartPoint(this->RightArm[i - 1].getEndpoint());
		this->RightArm[i].calculateEndpoint();

	}
	this->BigClawRight.setStartPoint(RightArm[3].getEndpoint());
	this->BigClawRight.calculateEndpoint();
	//for (int i = 0; i < 4; i++)
	//{
	//	if (timer.at(i)->IsTimeUp())
	//	{
	//		Shoot();
	//		timer.at(i)->ResetWithFixedStartTime(i*400);
	//		timer.at(i)->Start();
	//	}
	//}
	if (bursttimer->IsTimeUp())
	{
		if (bulletCount < 4)
		{
			if (!isWaitingToShoot)
			{
				shoottimer->Start();
				isWaitingToShoot = true;
			}

			if (shoottimer->IsTimeUp())
			{
				bulletCount++;
				Shoot();
				shoottimer->Reset();
				shoottimer->Start();
			}
		}
		else
		{
			isWaitingToShoot = false;
			bulletCount = 0;
			bursttimer->Reset();
			bursttimer->Start();
		}
	}

}


void CBoss::Render()
{
	//DebugOut(L"Boss render x: %f, y: %f \n",x,y);
	animationSet->at(0)->OldRender(x, y);
	//animationSet->at(0)->Render(x, y);

	if (state == BOSS_STATE_DIE) {
		return;
	}

	//RenderBoundingBox();
	BigClawLeft.Render();
	BigClawRight.Render();
	for (int i = 0; i < 4; i++)
	{
		LeftArm[i].Render();
	}
	for (int i = 0; i < 4; i++)
	{
		RightArm[i].Render();
	}
}

void CBoss::SetState(int state)
{
	Entity::SetState(state);
	switch (state)
	{
	case BOSS_STATE_DIE:
		//y += BOSS_BBOX_HEIGHT - BOSS_BBOX_HEIGHT_DIE + 1;
		//vx = 0;
		//vy = 0;
		break;
	case BOSS_STATE_WALKING_LEFT:

		break;
	case BOSS_STATE_WALKING_RIGHT:
		nx = 1;
		vx = BOSS_WALKING_SPEED;
	}
}

void CBoss::SetStartPosition(float x, float y)
{
	startX = x;
	startY = y;
}

void CBoss::getLeftClawPos(float& x, float& y)
{
	x = this->BigClawLeft.x;
	y = this->BigClawLeft.y;
}

void CBoss::getRightClawPos(float& x, float& y)
{
	x = this->BigClawRight.x;
	y = this->BigClawRight.y;
}

void CBoss::updateTarget1()
{
	lList[0] = Vec2(x, y) + Vec2(-180, -160);
	lList[1] = Vec2(x, y) + Vec2(20, 20);
	lList[2] = Vec2(x, y) + Vec2(20, 200);
	lList[3] = Vec2(x, y) + Vec2(-180, 200);
	lList[4] = Vec2(x, y) + Vec2(-100, 20);
	lList[5] = lList[1];
	lList[6] = Vec2(x, y) + Vec2(20, -160);
	lList[7] = Vec2(x, y) + Vec2(-180, -160);
	lList[8] = lList[4];
	lList[9] = lList[3];
	lList[10] = lList[4];
	lList[11] = lList[1];
	lList[12] = lList[5];
	lList[13] = lList[0];
	lList[14] = lList[3];
	lList[15] = lList[2];
	lList[16] = lList[1];
	lList[17] = lList[4];
	lList[18] = lList[0];




	const float speedX = 2.5f, speedY = 4.0f;
	nextTarget1 = lList[indexTarget1];
	if (Target1.x > nextTarget1.x)
		Target1.x -= speedX;
	else Target1.x += speedX;


	if (Target1.y > nextTarget1.y)
		Target1.y -= speedY;
	else Target1.y += speedY;
	if (Target1.Direction(nextTarget1).GetLength() < 10)
	{
		indexTarget1++;
	}
	if (indexTarget1 == 18)
	{
		indexTarget1 = 0;
	}
}

void CBoss::updateTarget2()
{

	//rList[0] = Vec2(x, y) + Vec2(180, -40);
	//rList[1] = Vec2(x, y) + Vec2(60, 80);
	//rList[2] = Vec2(x, y) + Vec2(40, 80);
	//rList[3] = Vec2(x, y) + Vec2(40, -40);
	//rList[4] = Vec2(x, y) + Vec2(60, 80);
	//rList[5] = Vec2(x, y) + Vec2(180, 60);

	rList[0] = Vec2(x, y) + Vec2(180, -160);
	rList[1] = Vec2(x, y) + Vec2(60, 200);
	rList[2] = Vec2(x, y) + Vec2(20, 200);
	rList[3] = Vec2(x, y) + Vec2(20, -160);
	rList[4] = Vec2(x, y) + Vec2(60, 200);
	rList[5] = Vec2(x, y) + Vec2(180, 180);

	const float speedX = 2.5f, speedY = 4.0f;
	nextTarget2 = rList[indexTarget2];
	if (Target2.x > nextTarget2.x)
		Target2.x -= speedX;
	else Target2.x += speedX;


	if (Target2.y > nextTarget2.y)
		Target2.y -= speedY;
	else Target2.y += speedY;
	if (Target2.Direction(nextTarget2).GetLength()<10)
	{
		indexTarget2++;
	}
	if (indexTarget2 ==6)
	{
		indexTarget2 = 0;
	}
	
}

void CBoss::Init()
{
	for (int i = 0; i < 4; i++)
	{
		this->LeftArm[i].setStartPoint(Vec2(x + 15, 120 + i * 15));
		this->LeftArm[i].calculateEndpoint();
		this->RightArm[i].setStartPoint(Vec2(x + 60 + i * 15, 120 + i * 15));
		this->RightArm[i].calculateEndpoint();
	}

}

void CBoss::Shoot()
{
	DebugOut(L"nkcjkjxzcn\n");
	Bullet* bullet = new BossBullet(this->x + 30, this->y + 20, target);
	CGrid::GetInstance()->InsertGrid(bullet);
}

void CBoss::BossClawSection::setStartPoint(Vec2 sp)
{
	this->startPoint = sp;
}

void CBoss::BossClawSection::calculateEndpoint()
{
	float delx = SectionLength * cos(Angle);
	float dely = SectionLength * sin(Angle);




	this->endPoint = Vec2(delx, dely) + startPoint;
	this->x = this->endPoint.x;
	this->y = this->endPoint.y;

}

void CBoss::BossClawSection::Follow(float x, float y)
{

	Vec2 Target = Vec2(x, y);
	//DebugOut(L"Target x %f ", x);
	//DebugOut(L"Target y %f ", y);
	Vec2 Direction = Target - this->startPoint;
	//	DebugOut(L"Direction x %f, ", Direction.x);
	//	DebugOut(L"Direction  y %f, ", Direction.y);
	Angle = atan2(Direction.y, Direction.x);
	//DebugOut(L"Angle %f", Angle);
	this->startPoint = Target + Direction.GetNormalized() * SectionLength * -1;


}

void CBoss::BossClawSection::Follow(Vec2& target)
{
	Follow(target.x, target.y);
}

void CBoss::BossClawSection::Follow(BossClawSection& target)
{
	Follow(target.startPoint.x, target.startPoint.y);

}

CBoss::BossClawSection::BossClawSection(int anisetid)
{
	this->x = 120;
	this->y = 240;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(anisetid));
	this->Angle = 0;
	this->setStartPoint(Vec2(120, 240));
	this->calculateEndpoint();


}
CBoss::BossClawSection::BossClawSection()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BOSS_CLAW));
	this->Angle = 0;
	this->startPoint = Vec2(200, 30);
	this->calculateEndpoint();

}

CBoss::BossClawSection::BossClawSection(Vec2 start, float angle)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BOSS_CLAW));
	this->Angle = angle;
	this->startPoint = start;
	this->calculateEndpoint();

}

void CBoss::BossClawSection::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 15;
	bottom = y + 15;
}

void CBoss::BossClawSection::Update(DWORD dt, vector<LPGAMEENTITY>* coObjects)
{

}

void CBoss::BossClawSection::Render()
{
	animationSet->at(0)->OldRender(x, y);
	//animationSet->at(0)->Render(x, y);

}



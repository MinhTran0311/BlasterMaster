#pragma once
#include "Boss.h"
#include "Utils.h"
#include "Game.h"
#include "Vec2.h"

int CBoss::injured_state_time = 0;
int CBoss::bossalpha = 255;
int CBoss::ani = 0;
CBoss::CBoss(float xPos, float yPos, LPGAMEENTITY t) :
	BigClawLeft(ANIMATION_SET_BOSS_CLAW_LEFT),
	BigClawRight(ANIMATION_SET_BOSS_CLAW_RIGHT)

{
	Target1= Vec2(10, 10);
	Target2= Vec2(10, 10);
	srand(time(NULL));
	x = xPos;
	y = yPos;
	startX = xPos;
	startY = yPos;
	explosiontimer->Start();
	target = t;
	tag = ENEMY;
	enemyType = BOSS;
	isActive = true;
	//isBoss = true;
	dam = 1;
	health = 10;
	nx = -1;
	SetState(BOSS_STATE_WALKING);
	vx = -BOSS_WALKING_SPEED;
	vy = BOSS_WALKING_SPEED;
	Init();

	bursttimer->Start();
}

void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	//this->isBoss = true;
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

	if (health <= 0) {
		SetState(BOSS_STATE_DIE);			//Handle Die state
	}

	if (state == BOSS_STATE_DIE) {
		//if (explosiontimerinit)
		//{
		//	explosiontimer->Start(); 
		//	explosiontimerinit = false;
		//}
		HandleDieState();
		return;
	}

	if (injured_state_time != 0) {
		injured_state_time = 0;
		injuredTimer->Reset();
		injuredTimer->Start();
	}
	HandleInjuredState();


#pragma region Pre-collision
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEENTITY> bricks;

	coEvents.clear();
	bricks.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (coObjects->at(i)->GetType() == EntityType::TAG_BRICK)
			bricks.push_back(coObjects->at(i));

		// turn off collision when die 
		if (state != BOSS_STATE_DIE)
			CalcPotentialCollisions(&bricks, coEvents);
	}
#pragma endregion
#pragma region coillision
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

		x += min_tx * dx + nx * 0.4f; // ko dính vào tường
		y += min_ty * dy + ny * 0.4f;// ko dính vào tường

		/*setRandomVxVy(vx, vy);*/
		if (!nx && !ny)
		{
			nx = -nx;
			vx = -vx;
			vy = -vy;
		}
		else if (!nx)
		{
			vy = -vy;
		}

		else if (!ny)
		{
			nx = -nx;
			vx = -vx;
		}




	}
	//clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion


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
			else if (shoottimer->IsTimeUp())
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
	animationSet->at(ani)->OldRender(x, y, bossalpha);
	//animationSet->at(0)->Render(x, y);



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
		ani = BOSS_ANI_DIE;
		break;
	case BOSS_STATE_WALKING:
		ani = BOSS_ANI_WALKING;
		//nx = 1;
		//vx = -BOSS_WALKING_SPEED;
		//vy = BOSS_WALKING_SPEED;
		break;
	case BOSS_STATE_INJURED:
		ani = BOSS_ANI_INJURED;
		injured_state_time = 1;
		break;
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
	lList[2] = Vec2(x, y) + Vec2(20, 120);
	lList[3] = Vec2(x, y) + Vec2(-180, 120);
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




	const float speedX = 2.5f, speedY = 3.5f;
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

	rList[0] = Vec2(x, y) + Vec2(120, -160);
	rList[1] = Vec2(x, y) + Vec2(60, 120);
	rList[2] = Vec2(x, y) + Vec2(20, 120);
	rList[3] = Vec2(x, y) + Vec2(20, -160);
	rList[4] = Vec2(x, y) + Vec2(60, 120);
	rList[5] = Vec2(x, y) + Vec2(120, 120);

	const float speedX = 2.5f, speedY = 3.5f;
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
	//for (int i = 0; i < 4; i++)
	//{
	//	this->LeftArm[i].setStartPoint(Vec2(x + 15, 120 + i * 15));
	//	this->LeftArm[i].calculateEndpoint();
	//	this->RightArm[i].setStartPoint(Vec2(x + 60 + i * 15, 120 + i * 15));
	//	this->RightArm[i].calculateEndpoint();
	//}

}

void CBoss::Shoot()
{
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
	this->x = 0;
	this->y = 0;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(anisetid));
	this->Angle = 0;
	this->setStartPoint(Vec2(120, 240));
	this->calculateEndpoint();


}
CBoss::BossClawSection::BossClawSection()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_BOSS_CLAW));
	this->Angle = 0;
	this->startPoint = Vec2(0, 0);
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
	//animationSet->at(0)->OldRender(x, y);
	////animationSet->at(0)->Render(x, y);

	int claw_ani;
	if (ani == BOSS_ANI_DIE)
		claw_ani = BOSS_ANI_INJURED;
	else
		claw_ani = ani;
	animationSet->at(claw_ani)->OldRender(x, y, bossalpha);

}


void CBoss::HandleInjuredState()
{


	if (!injuredTimer->IsTimeUp()) {
		ani = BOSS_ANI_INJURED;
	}
	else
	{
		injured_state_time = 0;
		ani = BOSS_ANI_WALKING;
		SetState(BOSS_STATE_WALKING);
	}
}

void CBoss::HandleDieState()
{
	
	if (numOfExplosion <= NUM_OF_EXPLOSION_APPEAR && explosiontimer->IsTimeUp())
	{
		explosiontimer->Reset();
		explosiontimer->Start();
		Explosion* exp = new Explosion();
		
		float randomX = random->getRandomFloat(-30.0f, 30.0f);
		float randomY = random->getRandomFloat(-30.0f, 30.0f);


		float posX, posY;
		posX = x + randomX;
		posY = y + randomY;
		//DebugOut(L"DUC explosion x:%f,y:%f, thutu:%d, tick: %d ", randomX, randomY, numOfExplosion, explosiontimer->GetTick());
		exp->Setposition(posX, posY);
		CGrid::GetInstance()->InsertGrid(exp);
		numOfExplosion++;
		bossalpha -= 15;
		
	}
	//alpha--;
	//if (alpha == 0) {
	//	//this->visible = false;
	//	//dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetEndingCount(); //Call EndingScene
	//}
	else {
		if (numOfExplosion> NUM_OF_EXPLOSION_APPEAR)
		{
			bossalpha = 0;
			isActive = false;
			dynamic_cast<Big_Sophia*>(target)->SetIsDoneFightWithBoss(true);
		}
		
	}
}
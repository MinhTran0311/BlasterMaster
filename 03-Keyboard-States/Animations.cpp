#include "Animations.h"

CAnimationSets* CAnimationSets::__instance = NULL;

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}



void CAnimation::OldRender(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}
	frames[currentFrame]->GetSprite()->OldDraw(x, y, alpha);
}

void CAnimation::RenderGunFlip(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 4;
		}
	}

	frames[currentFrame]->GetSprite()->OldDraw(x, y, alpha);
}

void CAnimation::RenderGunFlipTargetTop(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 4;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 4;
		}
	}

	frames[currentFrame]->GetSprite()->OldDraw(x, y, alpha);
}

int CAnimation::GetFrameStopWalking()
{
	switch (currentFrame)
	{
	case SOPHIA_STOP_WALKING_SPRITE1_OF_LIST:
	case SOPHIA_STOP_WALKING_SPRITE5_OF_LIST:
	case SOPHIA_STOP_WALKING_SPRITE9_OF_LIST:
		currentFrame = SOPHIA_STOP_WALKING_SPRITE2;
		break;
	case SOPHIA_STOP_WALKING_SPRITE2_OF_LIST:
	case SOPHIA_STOP_WALKING_SPRITE6_OF_LIST:
	case SOPHIA_STOP_WALKING_SPRITE10_OF_LIST:
		currentFrame = SOPHIA_STOP_WALKING_SPRITE3;
		break;
	case SOPHIA_STOP_WALKING_SPRITE7_OF_LIST:
	case SOPHIA_STOP_WALKING_SPRITE3_OF_LIST:
	case SOPHIA_STOP_WALKING_SPRITE11_OF_LIST:
		currentFrame = SOPHIA_STOP_WALKING_SPRITE4;
		break;
	default:
		currentFrame = SOPHIA_STOP_WALKING_SPRITE1;
		break;
	}
	return currentFrame;
}

void CAnimation::RenderFrame(int idFrame, float x, float y, int alpha)
{
	frames[idFrame]->GetSprite()->OldDraw(x, y, alpha);
}

void CAnimation::Render(int direction, float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}

	}
	frames[currentFrame]->GetSprite()->Draw(direction, x, y, alpha);
}

void CAnimation::RenderY(int direction, float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}

	}
	frames[currentFrame]->GetSprite()->DrawY(direction, x, y, alpha);
}

void CAnimation::RenderTopBottom(int direction, float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}

	}
	frames[currentFrame]->GetSprite()->DrawTopBottom(direction, x, y, alpha);
}


CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	return animations[id];
}


void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

void CAnimations::ClearAt(int i)
{
	LPANIMATION ani = animations[i];
	delete ani;
	animations.erase(i);
}

CAnimationSets::CAnimationSets()
{
}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}

LPANIMATION_SET CAnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void CAnimationSets::ClearAt(unsigned int i)
{
	LPANIMATION_SET aniset = animation_sets[i];
	delete aniset;
	animation_sets.erase(i);
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}
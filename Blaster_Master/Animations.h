#pragma once
#include "Sprites.h"

#define SOPHIA_STOP_WALKING_SPRITE1		0	//gom sprite thu 0 4 8
#define SOPHIA_STOP_WALKING_SPRITE2		1	//gom sprite thu 1 5 9
#define SOPHIA_STOP_WALKING_SPRITE3		2	//gom sprite thu 2 6 10
#define SOPHIA_STOP_WALKING_SPRITE4		7	//gom sprite thu 7 3 11

#define SOPHIA_STOP_WALKING_SPRITE0_OF_LIST			0
#define SOPHIA_STOP_WALKING_SPRITE1_OF_LIST			1
#define SOPHIA_STOP_WALKING_SPRITE2_OF_LIST			2
#define SOPHIA_STOP_WALKING_SPRITE3_OF_LIST			3
#define SOPHIA_STOP_WALKING_SPRITE4_OF_LIST			4
#define SOPHIA_STOP_WALKING_SPRITE5_OF_LIST			5
#define SOPHIA_STOP_WALKING_SPRITE6_OF_LIST			6
#define SOPHIA_STOP_WALKING_SPRITE7_OF_LIST			7
#define SOPHIA_STOP_WALKING_SPRITE8_OF_LIST			8
#define SOPHIA_STOP_WALKING_SPRITE9_OF_LIST			9
#define SOPHIA_STOP_WALKING_SPRITE10_OF_LIST		10
#define SOPHIA_STOP_WALKING_SPRITE11_OF_LIST		11

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);

	LPANIMATION_FRAME GetAnimationCurrentFrame(int id) { return frames[id]; }
	int GetFrameStopWalking();
	int GetFrame() { return currentFrame; }
	void SetFrame(int current_Frame) { currentFrame = current_Frame; }
	void ResetCurrentFrame() { currentFrame = -1; }
	void Render(int direction, float x, float y, int alpha = 255);
	void IntroRender(int direction, float x, float y, int alpha = 255);
	void RenderTopBottom(int direction, float x, float y, int alpha = 255);
	void RenderGunFlip(float x, float y, int alpha = 255);
	void RenderGunFlipTargetTop(float x, float y, int alpha = 255);
	void RenderFrame(int idFrame, float x, float y, int alpha = 255);
	int GetLastFrameIndex() { return frames.size() -1; };
};

typedef CAnimation* LPANIMATION;

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();
	void ClearAt(int i);

	static CAnimations* GetInstance();
};


typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets* __instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);
	void ClearAt(unsigned int i);

	static CAnimationSets* GetInstance();
};

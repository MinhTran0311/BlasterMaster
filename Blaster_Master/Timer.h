#pragma once

#include <Windows.h>

class Timer
{
	int startTime, limitTime;
public:
	Timer(int limitTime)
	{
		startTime = 0;
		this->limitTime = limitTime;
	}

	~Timer() {}

	int GetStartTime() { return startTime; }
	int GetLimitTime() { return limitTime; }
	int GetTick() { return GetTickCount() - startTime; };
	void Start() { startTime = GetTickCount(); }
	bool IsTimeUp() { return GetTickCount() - startTime >= limitTime; }
	void AddToTimer(int msec) { startTime += msec; }
	void Reset() { startTime = 0; }
	void Reset(int newLimitTime) { startTime = 0; limitTime = newLimitTime; }
};



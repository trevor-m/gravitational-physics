#pragma once
#include "SDL.h"

class Timer
{
private:
	unsigned int time;
	unsigned int count;
	bool started;
public:
	Timer();
	~Timer();
	void Start(unsigned int count);
	bool Done();
	bool Started();
	void Reset();
};


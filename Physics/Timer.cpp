#include "Timer.h"


Timer::Timer()
{
	time = SDL_GetTicks();
	count = 0;
	started = false;
}


Timer::~Timer()
{
}

void Timer::Start(unsigned int count)
{
	time = SDL_GetTicks();
	started = true;
	this->count = count;
}

bool Timer::Done()
{
	if (SDL_GetTicks() - time >= count)
		return true;
	return false;
}

bool Timer::Started()
{
	return started;
}

void Timer::Reset()
{
	time = SDL_GetTicks();
	count = 0;
	started = false;
}
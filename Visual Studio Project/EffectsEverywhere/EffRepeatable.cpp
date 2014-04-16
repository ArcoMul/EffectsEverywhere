#include "EffRepeatable.h"
#include <iostream>

EffRepeatable::EffRepeatable(std::function<void(void)> f, float startTime, float repeatAfter)
{
	this->f = f;
	this->startTime = startTime;
	this->repeatAfter = repeatAfter;
}

bool EffRepeatable::call (float time)
{
	// If the current time is bigger than the next time the function needs to be called
	if (time > startTime + (repeatAfter*1000))
	{
		// Call
		f();

		// Reset the last time the function was called for the next iteration
		startTime = time;

		return true;
	}
	return false;
}

EffRepeatable::~EffRepeatable(void)
{
}

#include "EffRepeatable.h"
#include "EffTimer.h"
#include <iostream>

EffRepeatable::EffRepeatable(std::function<void(void)> f, EffTimer* timer, float startTime, bool repeat, float repeatAfter)
{
	this->f = f;
	this->startTime = startTime;
	this->repeat = repeat;
	this->repeatAfter = repeatAfter;
	this->timer = timer;
}

bool EffRepeatable::call (float time)
{
	// If the current time is bigger than the next time the function needs to be called
	if (time > startTime + (repeatAfter*1000))
	{
		// Call
		f();

		// Remove the timer if it shouldn't be repeated
		if (!repeat) {
			timer->remove(this);
			return true;
		}

		// Reset the last time the function was called for the next iteration
		startTime = time;

		return true;
	}
	return false;
}

void EffRepeatable::stop(void)
{
	// Tell the timer to remove this repeatable
	timer->remove(this);
}

EffRepeatable::~EffRepeatable(void)
{
}

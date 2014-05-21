#include "EffTimer.h"
#include "EffScene.h"
#include "EffRepeatable.h"
#include <iostream>

EffTimer::EffTimer(EffScene* scene)
{
	this->scene = scene;
}

EffRepeatable* EffTimer::repeat (std::function<void(void)> f, float seconds)
{
	// Create a new repeatable based on the given paramaters
	EffRepeatable* r = new EffRepeatable(f, this, scene->getTime(), seconds);

	// Keep track of the repeatables
	repeatables.push_back(r);

	return r;
}

void EffTimer::update (float deltaTime)
{
	// Check if any tepeatable needs to be called
	for(auto repeatable = repeatables.begin(); repeatable != repeatables.end(); ++repeatable)
	{
		(**repeatable).call(scene->getTime());
	}
	cleanUpRepeatables();
}

void EffTimer::remove(EffRepeatable* r)
{
	// Add this repeatable to the back of the clean up list
	repeatablesToCleanUp.push_back(r);
}

void EffTimer::cleanUpRepeatables (void)
{
	// Remove all repeatables in the clean up list from the main list
	// TODO: should be possible more easily
	for(auto toCleanUp = repeatablesToCleanUp.begin(); toCleanUp != repeatablesToCleanUp.end(); ++toCleanUp)
	{
		for(auto repeatable = repeatables.begin(); repeatable != repeatables.end(); ++repeatable)
		{
			if ((*toCleanUp) == (*repeatable)) {
				repeatables.erase(repeatable);
				break;
			}
		}
	}
}

EffTimer::~EffTimer(void)
{
}

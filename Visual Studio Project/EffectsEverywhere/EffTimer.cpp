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
	cleanUp();
}

void EffTimer::remove(EffRepeatable* r)
{
	std::cout << "EffTimer remove EffRepeatable" << std::endl;
	cleanUpList.push_back(r);
	// Remove repeatable from list
}

void EffTimer::cleanUp (void)
{
	for(auto toCleanUp = cleanUpList.begin(); toCleanUp != cleanUpList.end(); ++toCleanUp)
	{
		for(auto repeatable = repeatables.begin(); repeatable != repeatables.end(); ++repeatable)
		{
			if ((*toCleanUp) == (*repeatable)) {
				std::cout << "EffTimer cleanUp EffRepeatable" << std::endl;
				repeatables.erase(repeatable);
				break;
			}
		}
	}
}

EffTimer::~EffTimer(void)
{
}

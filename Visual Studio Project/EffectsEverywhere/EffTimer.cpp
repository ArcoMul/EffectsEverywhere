#include "EffTimer.h"
#include "EffScene.h"
#include "EffRepeatable.h"
#include <iostream>

EffTimer::EffTimer(EffScene* scene)
{
	this->scene = scene;
}

void EffTimer::repeat (std::function<void(void)> f, float seconds)
{
	// Create a new repeatable based on the given paramaters
	EffRepeatable* r = new EffRepeatable(f, scene->getTime(), seconds);

	// Keep track of the repeatables
	repeatables.push_back(r);
}

void EffTimer::update (float deltaTime)
{
	// Check if any tepeatable needs to be called
	for(auto repeatable = repeatables.begin(); repeatable != repeatables.end(); ++repeatable)
	{
		(**repeatable).call(scene->getTime());
	}
}

EffTimer::~EffTimer(void)
{
}

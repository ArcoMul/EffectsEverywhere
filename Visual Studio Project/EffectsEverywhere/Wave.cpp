#include "MainScene.h"
#include "EffRepeatable.h"
#include "EffTimer.h"
#include "Wave.h"
#include <functional>

Wave::Wave(MainScene* scene, core::list<SpawnPoint> spawnpoints, float interval)
{
	this->scene = scene;
	this->onDone = nullptr;
	this->spawnpoints = spawnpoints;
	this->interval = interval;
}

void Wave::start ()
{
	// Set the repeating interval when starting the wave
	repeatable = scene->timer->repeat(std::bind(&Wave::spawnEnemies, this), interval);
}

void Wave::spawnEnemies ()
{
	// Boolean to track if anything was spawned this wave
	bool spawned = false;
	for(auto point = spawnpoints.begin(); point != spawnpoints.end(); ++point)
	{
		// Check if this spawnpoint has enemies left, if not skip this spawn point
		if (!(*point).hasEnemies()) {
			continue;
		}

		// There are enemies left to spawn, spawn one
		spawned = true;
		scene->spawnEnemy((*point).position, (*point).popEnemy());
	}

	// If nothing was spawned, stop the interval and tell the wave system that this
	//	wave is done
	if (!spawned) {
		repeatable->stop();
		if (onDone != nullptr) {
			onDone();
		}
	}
}

Wave::~Wave(void)
{
}

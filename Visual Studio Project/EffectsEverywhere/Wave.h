#ifndef WAVE_H_ 
#define WAVE_H_ 

#include "SpawnPoint.h"

class MainScene;
class EffRepeatable;

class Wave
{
public:
	/**
	 * Create a new wave with certain spawnpoints and a interval to spawn enemies
	 */
	Wave(MainScene* scene, core::list<SpawnPoint> spawnpoints, float interval);
	
	/**
	 * Tell this wave to start spawning enemies
	 */
	void start (void);

	/**
	 * Interval function to spawn enemies
	 */
	void spawnEnemies ();

	~Wave(void);

	/**
	 * Callback, execute when the wave is done
	 */
	std::function<void(void)> onDone;

private:
	/**
	 * Reference to the mainscene, to spawn enemies there
	 */
	MainScene* scene;

	/**
	 * List of spawnpoints of this wave, we spawn enemies at these points
	 */
	core::list<SpawnPoint> spawnpoints;
	
	/**
	 * The interval of spawning enemies, faster is more difficult
	 */
	float interval;

	/**
	 * Reference to the repeater / interval object, so that the interval can be stopped when it is done
     */
	EffRepeatable* repeatable;
};

#endif
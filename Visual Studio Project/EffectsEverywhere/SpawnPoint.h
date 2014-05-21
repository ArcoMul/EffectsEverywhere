#ifndef SPAWNPOINT_H_ 
#define SPAWNPOINT_H_

#include <irrlicht.h>
#include "Enemy.h"

class SpawnPoint
{
public:

	/**
	 * Create a spawn point based on a postion and which enemies to spawn
	 */
	SpawnPoint(core::vector2df position, core::list<int> enemies);

	/**
	 * Check if there are enemies left to spawn at this spawn point
     */
	bool hasEnemies(void);

	/**
	 * Get an enemy to spawn, based on the enemies left to spawn
	 */
	Enemy::TYPES popEnemy (void);

	~SpawnPoint(void);

	/**
	 * Position of this spawn point
	 */
	core::vector2df position;

	/**
	 * List with integers of how many enemies there are left to spawn
	 * 0 = Enemy::TYPES::<first> (PURPLE)
	 * 1 = Enemy::TYPES::<second> (EVIL)
	 */
	core::list<int> enemies;

private:
	
};

#endif
#include "SpawnPoint.h"

SpawnPoint::SpawnPoint(core::vector2df position, core::list<int> enemies)
{
	this->position = position;
	this->enemies = enemies;
}

bool SpawnPoint::hasEnemies()
{
	// If any of the types to spawn is bigger than zero; we have still something left to spawn
	for(auto enemyTypeToSpawn = enemies.begin(); enemyTypeToSpawn != enemies.end(); ++enemyTypeToSpawn)
	{
		if ((*enemyTypeToSpawn) > 0) return true;
	}
	return false;
}

Enemy::TYPES SpawnPoint::popEnemy (void)
{
	// Randomly spawn one of the types which are left in the enemies array
	int index = 0;
	for(auto enemyTypeToSpawn = enemies.begin(); enemyTypeToSpawn != enemies.end(); ++enemyTypeToSpawn)
	{
		if ((*enemyTypeToSpawn) > 0 && (rand() / (float) RAND_MAX) < (float) 1 / enemies.size()) {
			(*enemyTypeToSpawn)--;
			if (index == 0) {
				return Enemy::TYPES::PURPLE;
			} else {
				return Enemy::TYPES::EVIL;
			}
		}
		index++;
	}

	// If we randomly didnt find one this round, we do it again
	// WARNING: this could possible be an infinite loop, stack overflow etc.
	return popEnemy();
}

SpawnPoint::~SpawnPoint(void)
{
}

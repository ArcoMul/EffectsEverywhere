#include "SpawnPoint.h"

SpawnPoint::SpawnPoint(core::vector2df position, core::list<int> enemies)
{
	this->position = position;
	this->enemies = enemies;
}

bool SpawnPoint::hasEnemies()
{
	for(auto e = enemies.begin(); e != enemies.end(); ++e)
	{
		if ((*e) > 0) return true;
	}
	return false;
}

Enemy::TYPES SpawnPoint::popEnemy (void)
{
	std::cout << "Pop enemy" << std::endl;
	int i = 0;
	for(auto e = enemies.begin(); e != enemies.end(); ++e)
	{
    		std::cout << "Try index " << i << ", e = " << (*e) << "" << std::endl;
		if ((*e) > 0 && (rand() / (float) RAND_MAX) < (float) 1 / enemies.size()) {
			(*e)--;
			if (i == 0) {
				return Enemy::TYPES::NORMAL;
			} else {
				return Enemy::TYPES::EVIL;
			}
		}
		i++;
	}
	return popEnemy();
}

SpawnPoint::~SpawnPoint(void)
{
}

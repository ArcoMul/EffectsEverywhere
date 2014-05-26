#include "PurpleEnemy.h"

PurpleEnemy::PurpleEnemy (std::function<void(Enemy*)> onDie, scene::ISceneManager* manager, core::vector3df position, scene::ISceneNode* target, float speed)
	:Enemy(onDie, manager, position, target, speed)
{
	this->type = Enemy::TYPES::PURPLE;
	this->health = 10;
	this->speed = .04 + (0.03 * (rand() / (float) RAND_MAX));
	this->damage = 10;
	this->xp = 2;
	this->meshSrc = "../../Media/enemy.obj";
	this->spawnEffectSrc = "../../Media/spawn-effect-purple-enemy.xml";
}

PurpleEnemy::~PurpleEnemy(void)
{
}

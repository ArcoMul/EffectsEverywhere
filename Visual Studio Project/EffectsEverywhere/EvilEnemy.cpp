#include "EvilEnemy.h"

EvilEnemy::EvilEnemy (std::function<void(void)> onDie, scene::ISceneManager* manager, core::vector3df position, scene::ISceneNode* target, float speed)
	:Enemy(onDie, manager, position, target, speed)
{
	this->type = Enemy::TYPES::EVIL;
	this->health = 20;
	this->speed = .05 + (0.03 * (rand() / (float) RAND_MAX));
	this->damage = 20;
	this->meshSrc = "../../Media/enemy-devil.obj";
	this->spawnEffectSrc = "../../Media/spawn-effect-evil-enemy.xml";
}

EvilEnemy::~EvilEnemy(void)
{
}

#include "PurpleEnemy.h"
#include "EffEngine.h"

PurpleEnemy::PurpleEnemy (std::function<void(Enemy*)> onDie, scene::ISceneManager* manager, core::vector3df position, scene::ISceneNode* target, float speed)
	:Enemy(onDie, manager, position, target, speed)
{
	this->type = Enemy::TYPES::PURPLE;
	this->health = 10;
	this->speed = .04 + (0.03 * (rand() / (float) RAND_MAX));
	this->damage = 10;
	this->xp = 2;
	this->meshSrc = (char*) EffEngine::getPath("Media/enemy.obj").c_str();
	this->spawnEffectSrc = (char*) EffEngine::getPath("Media/spawn-effect-purple-enemy.xml").c_str();
}

PurpleEnemy::~PurpleEnemy(void)
{
}

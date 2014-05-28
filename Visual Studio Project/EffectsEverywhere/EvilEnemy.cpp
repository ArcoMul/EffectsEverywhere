#include "EvilEnemy.h"
#include "EffEngine.h"

EvilEnemy::EvilEnemy (std::function<void(Enemy*)> onDie, scene::ISceneManager* manager, core::vector3df position, scene::ISceneNode* target, float speed)
	:Enemy(onDie, manager, position, target, speed)
{
	this->type = Enemy::TYPES::EVIL;
	this->health = 20;
	this->speed = .05 + (0.03 * (rand() / (float) RAND_MAX));
	this->damage = 20;
	this->xp = 3;
	this->meshSrc = (char*) EffEngine::getPath("Media/enemy-devil.obj").c_str();
	this->spawnEffectSrc = (char*) EffEngine::getPath("Media/spawn-effect-evil-enemy.xml").c_str();
}

EvilEnemy::~EvilEnemy(void)
{
}

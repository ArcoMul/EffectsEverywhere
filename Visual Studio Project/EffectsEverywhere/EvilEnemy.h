#ifndef EVILENEMY_H_
#define EVILENEMY_H_

#include "Enemy.h"

class EvilEnemy : public Enemy
{
public:
	EvilEnemy (std::function<void(Enemy*)> onDie, scene::ISceneManager* manager, core::vector3df position, scene::ISceneNode* target, float speed = .05);
	~EvilEnemy (void);
};

#endif
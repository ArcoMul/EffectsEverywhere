#ifndef PURPLEENEMY_H_
#define PURPLEENEMY_H_

#include "Enemy.h"

class PurpleEnemy : public Enemy
{
public:
	PurpleEnemy (std::function<void(void)> onDie, scene::ISceneManager* manager, core::vector3df position, scene::ISceneNode* target, float speed = .05);
	~PurpleEnemy(void);
};

#endif
#ifndef ENEMY_H
#define ENEMY_H

#include "GameEngine.h"
#include "GameScene.h"

class Enemy
{
public:
	Enemy(GameEngine* engine, core::vector3df position);
	void addCollision (IMeshSceneNode* node);

	GameEngine* _engine;
private:
	IMeshSceneNode* node;
};

#endif

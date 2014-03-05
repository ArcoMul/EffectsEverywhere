#ifndef ENEMY_H
#define ENEMY_H

#include "GameEngine.h"
#include "GameScene.h"

class Enemy
{
public:
	// Create a constructor Enemy and give it the engine and position.
	Enemy(GameEngine* engine, core::vector3df position);

	// Create a constructor addCollision and give it a node
	void addCollision (IMeshSceneNode* node);

	GameEngine* _engine;
private:
	IMeshSceneNode* node;
};

#endif

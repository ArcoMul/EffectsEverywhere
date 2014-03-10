#ifndef ENEMY_H
#define ENEMY_H

#include "irrlicht.h"
#include "GameEngine.h"
#include "GameScene.h"

class Enemy
{
public:
	// Create a constructor Enemy and give it the engine and position.
	Enemy(GameEngine* engine, core::vector3df position, float speed = .05);

	void update(float deltaTime);

	// Create a constructor addCollision and give it a node
	void addCollision (IMeshSceneNode* node);

	bool collisionOccurred (core::vector3df* position);

	void setTarget (scene::ISceneNode* target);

	GameEngine* _engine;
private:
	scene::IMeshSceneNode* node;
	scene::ISceneNode* target;
	float speed;

	ISceneNodeAnimatorCollisionResponse* collision;
};

#endif

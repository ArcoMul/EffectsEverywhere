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

	bool hit (void);
	void die (void);

	bool isDeath;

	scene::IMeshSceneNode* node;
	GameEngine* _engine;
private:
	scene::ISceneNode* target;
	float speed;
	int hp;

	ISceneNodeAnimatorCollisionResponse* collision;
};

#endif

#ifndef ENEMY_H_
#define ENEMY_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class EffEngine;
class EffScene;

class Enemy : EffActor
{
public:
	// Create a constructor Enemy and give it the engine and position.
	Enemy(scene::ISceneManager* manager, core::vector3df position, float speed = .05);

	virtual void update (float deltaTime);

	// Add collision between this enemy and the given node
	void addCollision (scene::IMeshSceneNode* node);

	// Check if collision occured and tell what position it happened
	bool collisionOccurred (core::vector3df* position);

	// Set which node the enemy has to follow
	void setTarget (scene::ISceneNode* target);

	// When the enemy gets hit by a bullet
	bool hit (void);

	// When the health of the enemy is zero
	void die (void);

	// True if the player is death
	bool isDeath;

	scene::IMeshSceneNode* node;

	EffEngine* _engine;

private:
	// Which ISceneNode to follow when walking
	scene::ISceneNode* target;

	// Speed of walking of the enemy
	float speed;

	// Health of the enemy
	int health;

	// Keep track of the collision animator to check if there was collision
	scene::ISceneNodeAnimatorCollisionResponse* collision;

	scene::ISceneManager* manager;
};

#endif

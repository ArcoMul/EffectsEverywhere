#ifndef ENEMY_H_
#define ENEMY_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class EffEngine;
class EffScene;
class Robot;

class Enemy : public EffActor
{
public:
	
	/**
	 * Create a constructor Enemy and give it the engine and position.
	 */
	Enemy(scene::ISceneManager* manager, core::vector3df position, scene::ISceneNode* target, float speed = .05);

	virtual void start ();

	virtual void update (float deltaTime);

	/**
	 * Add collision between this enemy and the given node
	 */
	void addCollision (scene::IMeshSceneNode* node);

	/**
	 * Check if collision occured and tell what position it happened
	 */
	bool collisionOccurred (core::vector3df* position);

	/**
	 * When the enemy gets hit by a bullet
	 */
	bool hit (void);

	/**
	 * When the enemy hits the robot
	 */
	void hit (Robot* robot, core::vector3df position);

	/**
	 * When the health of the enemy is zero
	 */
	void die (void);

	~Enemy(void);

	bool followTarget;

	// True if the player is death
	bool isDeath;

	EffEngine* _engine;

private:
	/**
	 * Which ISceneNode to follow when walking
	 */
	scene::ISceneNode* target;

	/**
	 * Speed of walking of the enemy
	 */
	float speed;

	/**
	 * Health of the enemy
	 */
	int health;

	/**
	 * Cooldown to keep track when the enemy can 'hit' again
	 */
	float cooldown;

	/**
	 * Position where the enemy should spawn
	 */
	core::vector3df spawnPosition;

	/**
	 * Keep track of the collision animator to check if there was collision
	 */
	scene::ISceneNodeAnimatorCollisionResponse* collision;

	scene::ISceneManager* manager;
};

#endif

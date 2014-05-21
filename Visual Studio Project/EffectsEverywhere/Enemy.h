#ifndef ENEMY_H_
#define ENEMY_H_

#include <irrlicht.h>
#include <functional>
#include "EffActor.h"

using namespace irr;

class EffEngine;
class EffScene;
class Robot;

class Enemy : public EffActor
{
public:
	
	enum TYPES {
		PURPLE = 1,
		EVIL = 2
	};

	/**
	 * Create a constructor Enemy and give it the engine and position.
	 */
	Enemy(std::function<void(void)> onDie, scene::ISceneManager* manager, TYPES type, core::vector3df position, scene::ISceneNode* target, float speed = .05);

	virtual void start ();

	virtual void update (float deltaTime);

	/**
	 * Add collision between this enemy and the given node
	 */
	void addCollision (scene::ISceneNode* collisionNode, scene::IMesh* mesh);

	/**
	 * Check if collision occured and tell what position it happened
	 */
	bool collisionOccurred (core::vector3df* position);

	/**
	 * When the enemy gets hit by a bullet
	 */
	bool hit (int demage);

	/**
	 * When the enemy hits the robot
	 */
	void hit (Robot* robot, core::vector3df position);

	/**
	 * When the health of the enemy is zero
	 */
	void die (void);

	~Enemy(void);

	/**
	 * True if the player is death
	 */
	bool isDeath;

	EffEngine* _engine;

private:

	/**
	 * The function to call
	 */
	std::function<void(void)> onDie;

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

	Enemy::TYPES type;
};

#endif

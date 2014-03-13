#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include <irrlicht.h>
#include "EffScene.h"

using namespace irr;

class EffEngine;
class Enemy;
class Bullet;

class MainScene : public EffScene
{
public:
	MainScene();
	bool init (void);
	void update (float deltaTime);

	/**
	 * Spawns a particle at a certain position
	 * Give the position of the object to spawn the particle onto that object
	 */
	void spawnParticleEffect (core::vector3df position, core::stringc pathname);

	~MainScene(void);

	scene::ICameraSceneNode* camera;

	/** 
	 * boolean to regulate emitter of the particle
	 */
	bool hasEmitter;
	
private:

	void playerHit (core::vector3df hitPosition);
	void playerDie (void);

	scene::IMeshSceneNode* robot;
	scene::IParticleSystemSceneNode* particleSceneNode;
	scene::IParticleEmitter* Emitter;

	// Array with bullets
	Bullet* bullets[10];

	// Add which place in the bullets array to add the new bullet
	int bulletIndex;

	// Counter to keep track of the shoot cooldown
	float shootCooldown;

	Enemy* enemy1;
	Enemy* enemy2;

	float particleCooldown;

	bool particleOnCooldown;

	int playerHp;
	bool isPlayerDeath;

	// Triangle Selector and Animator Collision Response for the level collision.
	scene::ITriangleSelector* levelSelector;
	scene::ISceneNodeAnimatorCollisionResponse* collisionLevel;
};

#endif

#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include "GameEngine.h"
#include "GameScene.h"
#include "Bullet.h"
#include "Enemy.h"

class BackgroundFader;

class MainScene : public GameScene
{
public:
	MainScene(GameEngine* engine);
	void start (void);
	void update (void);

	/**
	 * Spawns a particle at a certain position
	 * Give the position of the object to spawn the particle onto that object
	 */
	void spawnParticleEffect (core::vector3df position, core::stringc pathname);
	~MainScene(void);

	ICameraSceneNode* camera;

	/** 
	 * boolean to regulate emitter of the particle
	 */
	bool hasEmitter;
	
private:

	void playerHit (core::vector3df hitPosition);
	void playerDie (void);

	IMeshSceneNode* robot;
	IParticleSystemSceneNode* particleSceneNode;
	IParticleEmitter* Emitter;

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

	ITriangleSelector* levelSelector;
	ISceneNodeAnimatorCollisionResponse* collisionLevel;
};

#endif

#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "GameEngine.h"
#include <irrlicht.h>

class GameScene
{
public:
	GameScene();
	GameScene(GameEngine* engine);
	virtual void start (void);
	virtual void update (void);

	scene::IParticleSystemSceneNode* particleSceneNode;
	scene::IParticleEmitter* Emitter;

	/** 
	 * boolean to regulate emitter of the particle
	 */
	bool hasEmitter;

	/**
	 * Cast ray to check for collision
	 * start, end -The start and end of the ray
	 * intersection -vector3df which is filled with the intersection of the collision
	 */
	bool checkRayCastIntersection(core::vector3df start, core::vector3df end, core::vector3df &intersection);

	/**
	 * Spawn mesh a certain position, only for debug purposes
	 */
	void spawnDebugMesh (core::vector3df position);

	/**
	 * Spawns a particle at a certain position
	 * Give the position of the object to spawn the particle onto that object
	 */
	void spawnParticleEffect (core::vector3df position);

	~GameScene(void);

	GameEngine* _engine;
};

#endif
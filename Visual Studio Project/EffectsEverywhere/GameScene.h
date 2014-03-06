#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "GameEngine.h"

class GameScene
{
public:
	GameScene();
	GameScene(GameEngine* engine);
	virtual void start (void);
	virtual void update (void);

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

	~GameScene(void);

	GameEngine* _engine;
};

#endif
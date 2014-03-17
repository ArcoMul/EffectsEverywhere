#ifndef MAINSCENE_H_
#define MAINSCENE_H_

#include <irrlicht.h>
#include "EffScene.h"

using namespace irr;

class EffEngine;
class Enemy;
class Robot;

class MainScene : public EffScene
{
public:
	MainScene();
	bool init (void);
	void update (float deltaTime);

	~MainScene(void);

	scene::ICameraSceneNode* camera;
	
private:

	void playerHit (core::vector3df hitPosition);
	void playerDie (void);

	Robot* robot;

	core::list<Enemy*> enemies;

	int playerHp;
	bool isPlayerDeath;

	// Triangle Selector and Animator Collision Response for the level collision.
	scene::ITriangleSelector* levelSelector;
	scene::ISceneNodeAnimatorCollisionResponse* collisionLevel;
};

#endif

#ifndef ROBOT_H_
#define ROBOT_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class EffEngine;
class Bullet;
class Enemy;

class Robot : public EffActor
{
public:
	Robot(void);

	virtual void update (float deltaTime);

	virtual void setNode (scene::ISceneNode* node);

	void shoot (core::list<Enemy*> enemies);

	~Robot(void);

private:
	// Counter to keep track of the shoot cooldown
	float shootCooldown;

	// Array with bullets
	Bullet* bullets[10];

	// Add which place in the bullets array to add the new bullet
	int bulletIndex;
};

#endif

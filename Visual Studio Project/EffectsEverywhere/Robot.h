#ifndef ROBOT_H_
#define ROBOT_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class EffEngine;
class Bullet;
class Enemy;
class Gun;

class Robot : public EffActor
{
public:
	Robot (void);

	virtual void start (void);

	virtual void update (float deltaTime);

	void shoot (core::list<Enemy*> enemies);

	void hit (core::vector3df position);

	~Robot(void);

	EffActor* mesh;

private:

	// Counter to keep track of the shoot cooldown
	float shootCooldown;

	// Array with bullets
	Bullet* bullets[10];

	// Add which place in the bullets array to add the new bullet
	int bulletIndex;

	Gun* gun;

	core::vector3df gunPosition;

	int floatDirection;
	float restFloatSpeed;
	float movingFloatSpeed;
	float floatSpeed;
};

#endif

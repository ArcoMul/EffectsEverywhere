#include "Robot.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "Enemy.h"
#include "Bullet.h"
#include "TemporaryParticleEffect.h"
#include "Gun.h"
#include <iostream>

Robot::Robot(void)
{

}

void Robot::start ()
{
	EffActor::start();
	node->setMaterialFlag(video::EMF_LIGHTING, false);

	gun = new Gun();
	scene->addMeshActor ((EffActor*) gun, "../../Media/rock-gun.obj");
	gun->node->setParent (node);

	core::matrix4 mat = node->getAbsoluteTransformation();
	core::vector3df right = core::vector3df(-mat[0], 0, -mat[2]);
	gun->node->setPosition(node->getPosition() + (right * 8.5) - core::vector3df(0, 4, 0));
}

void Robot::update(float deltaTime)
{
	EffActor::update(deltaTime);

	// Get the position of the robot
	core::vector3df pos = node->getPosition();

	// Get the rotation of the robot
	core::vector3df rot = node->getRotation();

	// Get the transformations done on this robot
	core::matrix4 mat = node->getAbsoluteTransformation();

	// Movement speed
 	float speed = .1;

 	// WARNING HACKY: When both a front/back key and a right/left key is pressed reduce the speed,
 	// so that it doesn't move twice as fast when going in a diagonal line
 	if ((scene->getInput()->IsKeyDown(irr::KEY_KEY_W) || scene->getInput()->IsKeyDown(irr::KEY_KEY_S))
 	&& (scene->getInput()->IsKeyDown(irr::KEY_KEY_A) || scene->getInput()->IsKeyDown(irr::KEY_KEY_D)))
 	speed *= 0.667; // 0.667 is sort of the factor of the distance you move when you go in a 45 degree angle
 
	// When the W key is down
	if(scene->getInput()->IsKeyDown(irr::KEY_KEY_W))
	{
		// Multiply the already done transformations of the robot with the speed and deltaTime
		pos += core::vector3df(mat[2] * speed * deltaTime,
		 	0,
			mat[0] * -speed * deltaTime);
	}
	// When the S key is down go back
	else if(scene->getInput()->IsKeyDown(irr::KEY_KEY_S))
	{
		pos += core::vector3df(mat[2] * -speed * deltaTime,
			0,
			mat[0] * speed * deltaTime);
	}
	
	// When the A key is down go right
	if(scene->getInput()->IsKeyDown(irr::KEY_KEY_A))
	{
		pos += core::vector3df(mat[0] * speed * deltaTime,
			0,
			mat[2] * speed * deltaTime);
	}
	// When the D key is down go left
	else if(scene->getInput()->IsKeyDown(irr::KEY_KEY_D))
	{
		pos += core::vector3df(mat[0] * -speed * deltaTime,
			0,
			mat[2] * -speed * deltaTime);
	}

	// Add deltaMouse, the change of mouse position, to the rotation of the robot
    rot.Y += -.3 * scene->getDeltaMouse().X;

	// Set the newly calculated position and rotation
	node->setPosition(pos);
	node->setRotation(rot);

	// Reduce the cooldown of shooting
	if (shootCooldown > 0) {
		shootCooldown -= deltaTime;
	}
}

void Robot::shoot (core::list<Enemy*>* enemies)
{
	if (shootCooldown > 0) return;

	core::matrix4 mat = node->getAbsoluteTransformation();

	// Calculate the start and end of the ray and pass the intersection variable to get the collision position
	core::vector3df intersection;
	core::vector3df forward = core::vector3df(mat[2], 0, -mat[0]);

	// Reset the cooldown
	shootCooldown = 350;

	// Create bullet actor with the right position and rotation
	Bullet* bullet = new Bullet(enemies);
	scene->addMeshActor ((EffActor*) bullet, "../../Media/rock-bullet.obj", gun->node->getAbsolutePosition(), node->getRotation());

	gun->shoot();
}

void Robot::hit (core::vector3df position)
{
	// Spawn a particle effect where the robot was hit
	TemporaryParticleEffect* p = new TemporaryParticleEffect(position, 500, "../../Media/portal1.bmp");
	scene->addParticleActor ((EffActor*) p);
}

Robot::~Robot(void)
{
}

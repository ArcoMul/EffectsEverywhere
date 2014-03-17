#include "Robot.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "Enemy.h"
#include "Bullet.h"

Robot::Robot(void)
{

}

void Robot::start ()
{
	EffActor::start();
	node->setMaterialFlag(video::EMF_LIGHTING, false);
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

void Robot::shoot (core::list<Enemy*> enemies)
{
	if (shootCooldown > 0) return;

	core::matrix4 mat = node->getAbsoluteTransformation();

	// Calculate the start and end of the ray and pass the intersection variable to get the collision position
	core::vector3df intersection;
	core::vector3df forward = core::vector3df(mat[2], 0, mat[0] * -1);

	// Set the beginning of the ray just a bit forward so that it doesnt hit the robot mesh
	scene::ISceneNode* intersectionNode = scene->checkRayCastIntersection(node->getPosition() + (forward * 5), node->getPosition() + (forward * 1000.), intersection);
	if (intersectionNode != nullptr && !scene->particleOnCooldown)
	{
		/** Spawn a particle at the place of the collision
			* the particle is created in the gamescene
			* Set the cooldown of the particle
			**/
		scene->spawnParticleEffect (intersection, "../../Media/fireball.bmp");
		scene->particleCooldown = 250;
		scene->particleOnCooldown = true;

		// Check which enemy was hit and tell the enemy it is hit, if the the hit function
		// returns true, it has to die
		for(core::list<Enemy*>::Iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++)
		{
			if ((*enemy)->node == intersectionNode) {
				(*enemy)->hit();
			}
		}
	}

	// Reset the cooldown
	shootCooldown = 350;

	// Create bullet actor with the right position and rotation
	Bullet* bullet = new Bullet(node->getPosition(), node->getRotation());
	scene->addMeshActor ((EffActor*) bullet, "../../Media/bullet.obj");
}

Robot::~Robot(void)
{
}

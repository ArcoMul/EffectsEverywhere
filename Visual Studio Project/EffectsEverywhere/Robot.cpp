#include "Robot.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "Enemy.h"
#include "Bullet.h"

Robot::Robot(void)
{
	// Set the whole bullet array at NULL so that we can check if the item is NULL or not later on
	for (int i = 0; i < 10; i++) {
		bullets[i] = NULL;
	}

	// Active bullet index starts at zero
	bulletIndex = 0;
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

	// Update all the bullets
	for (int i = 0; i < 10; i++)
	{
		if (bullets[i] == NULL) continue;

		// Remove the bullet if it is alive for 500 miliseconds
		if (scene->getTime() - bullets[i]->aliveSince > 500)
		{
			scene->removeActor((EffActor*) bullets[i]);
			delete bullets[i];
			bullets[i] = NULL;
			continue;
		}

		// Get the current position and rotation and calculate based on that the new position
		core::vector3df pos = bullets[i]->node->getPosition();
		core::matrix4 mat = bullets[i]->node->getAbsoluteTransformation();
		pos += core::vector3df(mat[2] * .5 * deltaTime,
			0,
			mat[0] * -.5 * deltaTime);
		bullets[i]->node->setPosition(pos);
	}

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

	// Create bullet actor
	Bullet* bullet = new Bullet(scene->getTime());
	scene->addMeshActor ((EffActor*) bullet, "../../Media/bullet.obj");

	// Give the bullet mesh the right material and position
	bullet->node->setMaterialFlag(video::EMF_LIGHTING, false);
	bullet->node->setPosition(node->getPosition());
	bullet->node->setRotation(node->getRotation());

	// If there is already a bullet in the bullet array on this position, remove that bullet
	if (bullets[bulletIndex] != NULL) {
		bullets[bulletIndex]->node->remove();
		delete bullets[bulletIndex];
		bullets[bulletIndex] = NULL;
	}

	// Add the bullet to the bullet array on the active index
	bullets[bulletIndex] = bullet;

	// Add one to the active bullet index or reset it to zero
	if (bulletIndex < 9) {
		bulletIndex++;
	} else {
		bulletIndex = 0;
	}
}

void Robot::setNode(scene::ISceneNode* node)
{
	EffActor::setNode(node);
	node->setMaterialFlag(video::EMF_LIGHTING, false);
}

Robot::~Robot(void)
{
}

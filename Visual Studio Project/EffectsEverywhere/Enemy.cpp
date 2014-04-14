#include "Enemy.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "Robot.h"
#include <iostream>
#include <cmath>

Enemy::Enemy(scene::ISceneManager* manager, core::vector3df position, scene::ISceneNode* target, float speed)
{
	this->manager = manager;
	this->target = target;
	this->speed = speed;
	this->health = 5;
	this->followTarget = false;
	this->isDeath = false;
	this->spawnPosition = position;
	this->cooldown = 0;
}

void Enemy::start ()
{
	EffActor::start ();

	// Get the mesh
	scene::IMesh* meshEnemy = manager->getMesh("../../Media/enemy.obj");
	node = manager->addOctreeSceneNode(meshEnemy, 0);

	// Set the right lightning and position
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setPosition (spawnPosition);

	// Give the enemies a triangle selector for ray cast detecting of bullets
	scene::ITriangleSelector* selector = manager->createOctreeTriangleSelector(meshEnemy, node, 12);
	node->setTriangleSelector(selector);
}

void Enemy::update(float deltaTime)
{
	EffActor::update(deltaTime);

	// Someting goes wrong when the target is set from the start of the game
	// so we set it when W is pressed once
	if (!followTarget && scene->getInput()->IsKeyDown(irr::KEY_KEY_W)) {
		followTarget = true;
	}
	
	// When there is no target, we don't want to follow the target, or the enemy is death; stop
	if (target == nullptr || !followTarget || isDeath) return;

	core::vector3df pos = node->getPosition();

	// The direction vector
	core::vector3df direction = pos - target->getPosition();

	// Angle based on the direction vector
	float angle = atan2 (direction.X, direction.Z) * 180 / (atan(1.)*4);

	// Set the right rotation for the enemy
	node->setRotation(core::vector3df(0, angle + 180, 0));

	// As long as there is no collision, walk in the direction of the target
	if (collision->collisionOccurred()) return;

	// Walk forwards using the transformation matrix
	core::matrix4 mat = node->getAbsoluteTransformation();
	pos -= core::vector3df(mat[2] * speed * deltaTime,
			0,
			mat[0] * -speed * deltaTime);

	// WARNING: hacky; collision influences Y position, reset this one to prevent weird behaviour
	pos.Y = 0;

	node->setPosition(pos);

	cooldown -= deltaTime;
}

bool Enemy::collisionOccurred (core::vector3df* position)
{
	if (collision->collisionOccurred()) {
		*position = collision->getCollisionPoint();
		return true;
	}
	return false;
}

/**
 * Tell an object it should collide with this object
 * (we might want to move this function to that object, where we add the collision to instead of this object
 */
void Enemy::addCollision (scene::ISceneNode* collisionNode, scene::IMesh* mesh)
{
	// Create an octree triangle selector for collision detection and attach it to our node.
	scene::ITriangleSelector* selector = manager->createOctreeTriangleSelector(mesh, collisionNode, 12);
	collisionNode->setTriangleSelector(selector);

	// Add an animator to the camera, a Collision Response Animator. This animator prevents
	// your object (player) to move through walls and other objects. The collision box of the enemy
	// has been set to 7, 7, 7. We do nothing with the gravity, this is why we set the vector to 0, 0, 0.
	// The last vector is a translation for the animator, which is set to 0, 0, 1.
	collision = manager->createCollisionResponseAnimator(
			selector, node, core::vector3df(7, 7, 7),
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 1));

	// We add the animator to our collisionNode and drop the selector and collision if
	// we don't need it anymore.
	node->addAnimator((scene::ISceneNodeAnimator*) collision);
	selector->drop();
	collision->drop();
}

bool Enemy::hit (int damage)
{
	health -= damage;
	if (health <= 0) {
		die ();
		return true;
	} else {
		return false;
	}
}

void Enemy::hit (Robot* robot, core::vector3df position)
{
	if (cooldown <= 0)
	{
		robot->hit(position);
		cooldown = 500;
	}
}

void Enemy::die ()
{
	isDeath = true;
	scene->removeActor ((EffActor*) this);
}

Enemy::~Enemy()
{
}
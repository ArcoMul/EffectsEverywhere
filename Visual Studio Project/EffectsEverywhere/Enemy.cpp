#include "Enemy.h"
#include <iostream>
#include <math.h> 

Enemy::Enemy(GameEngine* engine, core::vector3df position, float speed)
{
	this->_engine = engine;
	this->target = nullptr;
	this->speed = speed;
	this->health = 5;
	this->isDeath = false;

	// Get the mesh
	IMesh* meshEnemy = engine->smgr->getMesh("../../Media/enemy.obj");
	node = engine->smgr->addOctreeSceneNode(meshEnemy, 0);

	// Set the right lightning and position
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setPosition(position);
	node->setName ("enemy");

	// Give the enemies a triangle selector for ray cast detecting of bullets
	ITriangleSelector* selector = _engine->smgr->createOctreeTriangleSelector(meshEnemy, node, 12);
	node->setTriangleSelector(selector);
}

void Enemy::update(float deltaTime)
{
	if (target == nullptr || isDeath) return;

	core::vector3df pos = node->getPosition();

	// The direction vector
	core::vector3df direction = pos - target->getPosition();

	// Angle based on the direction vector
	float angle = atan2 (direction.X, direction.Z) * 180 / PI;

	// Set the right rotation for the enemy
	node->setRotation(core::vector3df(0, angle + 180, 0));

	// As long as there is no collision, walk in the direction of the target
	if (collision->collisionOccurred()) return;

	// Walk forwards using the transformation matrix
	core::matrix4 mat = node->getAbsoluteTransformation();
	pos -= core::vector3df(mat[2] * speed * _engine->deltaTime,
			0,
			mat[0] * -speed * _engine->deltaTime);

	// WARNING: hacky; collision influences Y position, reset this one to prevent weird behaviour
	pos.Y = 0;

	node->setPosition(pos);
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
void Enemy::addCollision (IMeshSceneNode* collisionNode)
{
	// Create an octree triangle selector for collision detection and attach it to our node.
	ITriangleSelector* selector = _engine->smgr->createOctreeTriangleSelector(collisionNode->getMesh(), collisionNode, 12);
	collisionNode->setTriangleSelector(selector);

	// Add an animator to the camera, a Collision Response Animator. This animator prevents
	// your object (player) to move through walls and other objects. The collision box of the enemy
	// has been set to 7, 7, 7. We do nothing with the gravity, this is why we set the vector to 0, 0, 0.
	// The last vector is a translation for the animator, which is set to 0, 0, 1.
	collision = _engine->smgr->createCollisionResponseAnimator(
			selector, node, core::vector3df(7, 7, 7),
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 1));

	// We add the animator to our collisionNode and drop the selector and collision if
	// we don't need it anymore.
	node->addAnimator((scene::ISceneNodeAnimator*) collision);
	selector->drop();
	collision->drop();
}

void Enemy::setTarget (ISceneNode* target)
{
	this->target = target;
}

bool Enemy::hit ()
{
	health -= 1;
	if (health <= 0) {
		die ();
		return true;
	} else {
		return false;
	}
}

void Enemy::die ()
{
	isDeath = true;
	node->remove();
}
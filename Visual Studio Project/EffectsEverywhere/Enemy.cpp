#include <iostream>
#include "Enemy.h"
#include "irrlicht.h"

Enemy::Enemy(GameEngine* engine, core::vector3df position)
{
	this->_engine = engine;

	// Get the mesh
	IMesh* meshEnemy = engine->smgr->getMesh("../../Media/enemy.obj");
	node = engine->smgr->addOctreeSceneNode(meshEnemy, 0);

	// Set the right lightning and position
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setPosition(position);
}

/**
 * Tell an object it should collide with this object
 * (we might want to move this function to that object, where we add the collision to instead of this object
 */
void Enemy::addCollision (IMeshSceneNode* collisionNode)
{
	// Create an octree triangle selector for collision detection and attach it to our node.
	ITriangleSelector* selector = _engine->smgr->createOctreeTriangleSelector(node->getMesh(), node, 12);
	node->setTriangleSelector(selector);

	// Add an animator to the camera, a Collision Response Animator. This animator prevents
	// your object (player) to move through walls and other objects. The collision box of the enemy
	// has been set to 7, 7, 7. We do nothing with the gravity, this is why we set the vector to 0, 0, 0.
	// The last vector is a translation for the animator, which is set to 0, 0, 1.
	ISceneNodeAnimatorCollisionResponse* collision = _engine->smgr->createCollisionResponseAnimator(
			selector, collisionNode, core::vector3df(7, 7, 7),
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 1));

	// We add the animator to our collisionNode and drop the selector and collision if
	// we don't need it anymore.
	collisionNode->addAnimator((scene::ISceneNodeAnimator*) collision);
	selector->drop();
	collision->drop();
}
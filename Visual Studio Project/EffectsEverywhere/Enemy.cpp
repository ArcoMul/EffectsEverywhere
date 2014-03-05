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
	ITriangleSelector* selector = _engine->smgr->createOctreeTriangleSelector(node->getMesh(), node, 12);
	node->setTriangleSelector(selector);

	ISceneNodeAnimatorCollisionResponse* collision = _engine->smgr->createCollisionResponseAnimator(
			selector, collisionNode, core::vector3df(7, 7, 7),
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 1));

	collisionNode->addAnimator((scene::ISceneNodeAnimator*) collision);
	selector->drop();
	collision->drop();
}
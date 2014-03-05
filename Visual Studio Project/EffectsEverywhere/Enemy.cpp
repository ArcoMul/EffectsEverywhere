#include <iostream>
#include "Enemy.h"
#include "irrlicht.h"

Enemy::Enemy(GameEngine* engine, ICameraSceneNode* _camera, IMeshSceneNode* robot) {

	this->_engine = engine;

	IAnimatedMesh* meshEnemy = _engine->smgr->getMesh("../../Media/Wall.obj");
	IMeshSceneNode* node = _engine->smgr->addOctreeSceneNode(meshEnemy, 0);
	ITriangleSelector* selector = _engine->smgr->createOctreeTriangleSelector(node->getMesh(), node, 12);
	node->setTriangleSelector(selector);
	node->setPosition(core::vector3df(0, 10, -55));

	ISceneNodeAnimatorCollisionResponse* collision = _engine->smgr->createCollisionResponseAnimator(
			selector, _camera, core::vector3df(7, 7, 7),
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 1));

	scene::ISceneNodeAnimator* animator = (scene::ISceneNodeAnimator*) collision;
	selector->drop();

	robot->addAnimator(animator);
	animator->drop();
}
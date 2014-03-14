#include "EffScene.h"
#include "EffEngine.h"
#include "InputReceiver.h"
#include "EffActor.h"
#include <iostream>

EffScene::EffScene()
{
}

EffScene::EffScene(EffEngine* engine) :
	engine(engine)
{
}

bool EffScene::init(void)
{
	actors = core::list<EffActor*>();
	return true;
}

void EffScene::update(float deltaTime)
{
	for(core::list<EffActor*>::Iterator a = actors.begin(); a != actors.end(); a++)
	{
		(*a)->update(deltaTime);
	}
}

EffActor* EffScene::addActor(EffActor* actor)
{
	actors.push_back (actor);
	return actor;
}

void EffScene::setEngine (EffEngine* engine)
{
	this->engine = engine;
}

void EffScene::setManager(scene::ISceneManager* manager)
{
	this->manager = manager;
}

video::ITexture* EffScene::getTexture (core::stringc pathname)
{
	return engine->driver->getTexture(pathname);
}

void EffScene::setMouseVisible (bool mouseVisible)
{
	engine->setMouseVisible(mouseVisible);
}

InputReceiver* EffScene::getInput ()
{
	return engine->inputReceiver;
}

core::vector2di EffScene::getDeltaMouse ()
{
	return engine->deltaMouse;
}

const irr::u32 EffScene::getTime()
{
	return engine->device->getTimer()->getTime();
}

scene::ISceneNode* EffScene::checkRayCastIntersection (core::vector3df start, core::vector3df end, core::vector3df &intersection)
{
	// Create ray with start and endpoint
	core::line3d<f32> ray;
	ray.start = start;
	ray.end = end;

	// Used to show with triangle has been hit
	core::triangle3df hitTriangle;

	// Checks collision for all node
	scene::ISceneNode* selectedSceneNode = engine->smgr->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(
		ray,
		intersection, // Position of the collision
		hitTriangle, // Triangle hit in the collision
		0,
		0);

	// We hit something
	if(selectedSceneNode)
	{
		return selectedSceneNode;
	}

	return nullptr;
}

void EffScene::spawnDebugMesh (core::vector3df position) 
{
	scene::IMesh* debugMesh = engine->smgr->getMesh("../../Media/debug-sphere.obj");
	scene::IMeshSceneNode* debugNode = engine->smgr->addMeshSceneNode(debugMesh);
	debugNode->setMaterialFlag(video::EMF_LIGHTING, false);
	debugNode->setPosition(position);
}

EffScene::~EffScene(void)
{
}

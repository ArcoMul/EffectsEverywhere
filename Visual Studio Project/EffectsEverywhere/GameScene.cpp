#include "GameScene.h"
#include <iostream>

GameScene::GameScene()
{
}

GameScene::GameScene(GameEngine* engine) :
	_engine(engine)
{
}

void GameScene::start(void)
{
	
}

void GameScene::update(void)
{	
}

bool GameScene::checkRayCastIntersection (core::vector3df start, core::vector3df end, core::vector3df &intersection)
{
	// Create ray with start and endpoint
	core::line3d<f32> ray;
	ray.start = start;
	ray.end = end;

	// Used to show with triangle has been hit
	core::triangle3df hitTriangle;

	// Checks collision for all node
	scene::ISceneNode * selectedSceneNode = _engine->smgr->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(
		ray,
		intersection, // Position of the collision
		hitTriangle, // Triangle hit in the collision
		0,
		0);

	// We hit something
	if(selectedSceneNode)
	{
		return true;
	}

	return false;
}

void GameScene::spawnDebugMesh (core::vector3df position) 
{
	IMesh* debugMesh = _engine->smgr->getMesh("../../Media/debug-sphere.obj");
	IMeshSceneNode* debugNode = _engine->smgr->addMeshSceneNode(debugMesh);
	debugNode->setMaterialFlag(EMF_LIGHTING, false);
	debugNode->setPosition(position);
}

GameScene::~GameScene(void)
{
}

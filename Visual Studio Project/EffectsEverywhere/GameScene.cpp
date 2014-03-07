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
	hasEmitter = true;
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

void GameScene::spawnParticleEffect (core::vector3df position) 
{
	//creating a particlesystemscenenode which basicly is a particle
	particleSceneNode = _engine->smgr->addParticleSystemSceneNode(false);
	
	//creating an emitter so u actually emit the particle from somewhere so it will be visual( in this case it's a box )
	Emitter = particleSceneNode->createBoxEmitter(aabbox3df(-3, 0, -3, 3, 1, 3 ),vector3df(0.0f, 0.1f, 0.0f),50,200,SColor(0, 0, 0, 255),
		SColor(0,255,255,255),500,750,0,dimension2df(4.0f, 4.0f), dimension2df(8.0f, 8.0f));

	// adding a particle affector which causes the particles to fade out
	IParticleAffector* paf = particleSceneNode->createFadeOutParticleAffector();

    particleSceneNode->addAffector(paf); // same goes for the affector
    paf->drop();

	//add the emitter to the particle and drop to prevent memory leakage
	particleSceneNode->setEmitter(Emitter);
	//Emitter->drop();

	//check if the particlesystemscenenode is created correctly
	if(particleSceneNode){
		particleSceneNode->setPosition(position);
		particleSceneNode->setScale(vector3df(0.5f, 0.5f,0.5f));
		particleSceneNode->setMaterialFlag(EMF_LIGHTING, false);
		particleSceneNode->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		particleSceneNode->setMaterialTexture(0, _engine->driver->getTexture("../../Media/fireball.bmp"));
		particleSceneNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	}
}

GameScene::~GameScene(void)
{
}

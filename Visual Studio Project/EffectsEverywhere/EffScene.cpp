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
	actor->setScene (this);
	return actor;
}

EffActor* EffScene::addMeshActor(EffActor* actor, core::stringc meshPath)
{
	addActor(actor);

	scene::IMesh* mesh = manager->getMesh(meshPath);
	scene::ISceneNode* node = (scene::ISceneNode*) manager->addMeshSceneNode(mesh);
	actor->setNode (node);
	return actor;
}

void EffScene::removeActor(EffActor* actor)
{
	for(core::list<EffActor*>::Iterator a = actors.begin(); a != actors.end(); a++)
	{
		if ((*a) == actor)
		{
			actor->node->remove();
			actors.erase (a);
			break;
		}
	}
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

void EffScene::spawnParticleEffect (core::vector3df position, core::stringc pathname) 
{
	//creating a particlesystemscenenode which basicly is a particle
	particleSceneNode = manager->addParticleSystemSceneNode(false);
	
	//creating an emitter so u actually emit the particle from somewhere so it will be visual( in this case it's a box )
	Emitter = particleSceneNode->createBoxEmitter(aabbox3df(-3, 0, -3, 3, 1, 3 ),vector3df(0.0f, 0.1f, 0.0f),50,200,SColor(0, 0, 0, 255),
		SColor(0,255,255,255),500,750,0,dimension2df(4.0f, 4.0f), dimension2df(8.0f, 8.0f));

	// adding a particle affector which causes the particles to fade out
	scene::IParticleAffector* paf = particleSceneNode->createFadeOutParticleAffector();
    
	particleSceneNode->addAffector(paf); // same goes for the affector
    paf->drop();

	//add the emitter to the particle and drop to prevent memory leakage
	particleSceneNode->setEmitter(Emitter);

	//check if the particlesystemscenenode is created correctly
	if(particleSceneNode)
	{
		particleSceneNode->setPosition(position);
		particleSceneNode->setScale(vector3df(0.5f, 0.5f,0.5f));
		particleSceneNode->setMaterialFlag(EMF_LIGHTING, false);
		particleSceneNode->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		particleSceneNode->setMaterialTexture(0, getTexture(pathname));
		particleSceneNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	}
}

EffScene::~EffScene(void)
{
}

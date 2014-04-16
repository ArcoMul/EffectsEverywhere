#include "EffScene.h"
#include "EffEngine.h"
#include "InputReceiver.h"
#include "EffActor.h"
#include <iostream>
#include <ParticleManager.h>

EffScene::EffScene()
{
}

EffScene::EffScene(EffEngine* engine) :
	engine(engine)
{
}

bool EffScene::init(void)
{
	pManager = new ParticleManager(engine->driver, engine->device, engine->smgr);
	actors = core::list<EffActor*>();
	actorsToRemove = core::list<EffActor*>();
	return true;
}

void EffScene::update(float deltaTime)
{
	// Call the update function of each actor
	for(core::list<EffActor*>::Iterator actor = actors.begin(); actor != actors.end(); actor++)
	{
		(*actor)->update(deltaTime);
	}

	// Some actors are saved to be removed, remove these now after we updated all the actors
	cleanupActors ();
}

EffActor* EffScene::addActor (EffActor* actor, bool start)
{
	// Add the actor to the actors array of this scene
	actors.push_back (actor);

	// Tell the actor in which scene it is
	actor->setScene (this);

	// Done, tell the actor everything is set and ready
	if (start) actor->start ();

	return actor;
}

EffActor* EffScene::addNodeActor(EffActor* actor, core::vector3df position, core::vector3df rotation)
{
	// Add the actor to the current scene
	actor = addActor(actor, false);

	// Create a mesh using the given source path
	scene::ISceneNode* node = (scene::ISceneNode*) manager->addSceneNode("empty", manager->getRootSceneNode());
	node->setPosition (position);
	node->setRotation (rotation);

	// Tell the actor which irrlicht node belongs to him
	actor->setNode (node);

	// Done, tell the actor everything is set and ready
	actor->start ();

	return actor;
}

EffActor* EffScene::addMeshActor(EffActor* actor, core::stringc meshPath, bool start)
{
	// Add the actor to the current scene
	actor = addActor(actor, false);

	// Create a mesh using the given source path
	scene::IMesh* mesh = manager->getMesh(meshPath);
	scene::ISceneNode* node = (scene::ISceneNode*) manager->addMeshSceneNode(mesh);

	// Tell the actor which irrlicht node belongs to him
	actor->setNode (node);

	// Done, tell the actor everything is set and ready
	if (start) actor->start ();

	return actor;
}

EffActor* EffScene::addMeshActor(EffActor* actor, core::stringc meshPath, core::vector3df position, bool start)
{
	actor = addMeshActor (actor, meshPath, false);

	// Set the node at the given position
	actor->node->setPosition (position);

	// Done, tell the actor everything is set and ready
	if(start) actor->start ();

	return actor;
}

EffActor* EffScene::addMeshActor(EffActor* actor, core::stringc meshPath, core::vector3df position, core::vector3df rotation)
{
	actor = addMeshActor (actor, meshPath, position, false);

	// Set the node at the given position
	actor->node->setRotation (rotation);

	// Done, tell the actor everything is set and ready
	actor->start ();

	return actor;
}

/**
 * Creates a particle actor based on a ParticleModel
 */
EffActor* EffScene::addParticleActor(EffActor* actor, ParticleModel* model, core::vector3df position)
{
	// Add the actor to the current scene
	actor = addActor(actor, false);

	// Create the particle system scene node
	scene::IParticleSystemSceneNode* particleNode = pManager->spawnDataModelParticle(model, position, model->getPathNameTexture());
	
	// Tell the actor which irrlicht node belongs to him
	actor->setNode ((scene::ISceneNode*) particleNode);

	// Done, tell the actor everything is set and ready
	actor->start ();

	return actor;
}

void EffScene::addActorList (core::list<EffActor*>* list)
{
	actorLists.push_back(list);
}

void EffScene::removeActor(EffActor* actor)
{
	// Save that this actors has to be removed
	actorsToRemove.push_back (actor);
}

bool EffScene::removeActorFromList (EffActor* actorToRemove, core::list<EffActor*>* list)
{
	// Loop through all the actors in the scene
	// TODO: loop in a loop, make more optimal using a find function or some sort of
	for(auto actor = list->begin(); actor != list->end(); ++actor)
	{
		// Find the actor wich matches with the actor which needs to be removed
		if ((*actor) == actorToRemove)
		{
			// Erase the actor from the actors list
			list->erase(actor);
			return true;
		}
	}
	return false;
}

void EffScene::cleanupActors ()
{
	// Loop through all the actors which needs to be removed
	for(core::list<EffActor*>::Iterator actorToRemove = actorsToRemove.begin(); actorToRemove != actorsToRemove.end(); actorToRemove++)
	{
		// Remove the actor from the main list
		if (removeActorFromList((*actorToRemove), &actors))
		{
			// If the actor is found, tell also the actor to delete itself
			delete (*actorToRemove);
		}

		// Remove the actor from other actor lists we keep track of
		for(auto it = actorLists.begin(); it != actorLists.end(); ++it)
		{
			removeActorFromList((*actorToRemove), (*it));
		}
	}

	// All actors which needed to be removed are removed, clear the to remove list
	actorsToRemove.clear();
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

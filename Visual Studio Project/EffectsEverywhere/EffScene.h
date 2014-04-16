#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <irrlicht.h>
#include <ParticleManager.h>
#include <ParticleModel.h>

using namespace irr;

class EffEngine;
class InputReceiver;
class EffActor;
class ParticleManager;
class ParticleModel;
class EffTimer;

class EffScene
{
public:
	EffScene();

	EffScene(EffEngine* engine);

	virtual bool init (void);

	virtual void update (float deltaTime);

	virtual void stop ();

	/**
	 * Add a default actor to the scene
	 */
	EffActor* addActor (EffActor* actor, bool start = true);

	EffActor* addNodeActor (EffActor* actor, core::vector3df position, core::vector3df rotation);

	/**
	 * Add an actor to the scene where we add a mesh to and set a IMeshSceneNode
	 */
	EffActor* addMeshActor (EffActor* actor, core::stringc meshPath, bool start = true);

	/**
	 * Add MeshActor and set position by default, also one with rotation as extra parameter
	 */
	EffActor* addMeshActor (EffActor* actor, core::stringc meshPath, core::vector3df position, bool start = true);
	EffActor* addMeshActor (EffActor* actor, core::stringc meshPath, core::vector3df position, core::vector3df rotation);

	/**
	 * Add an actor to the scene where we add a emitter to and set a IParticleSystemSceneNode
	 */
	EffActor* addParticleActor(EffActor* actor, core::vector3df position);

	// TODO: temporary, see .cpp file
	EffActor* addPointParticleActor(EffActor* actor, core::vector3df position);

	// new way
	EffActor* addParticleActor(EffActor* actor, ParticleModel* model, core::vector3df position);

	/**
	 * Public function to record that an actors has to be removed, this
	 * actors will be removed from the scene at the end of the execution
	 * of this frame. (We can't remove an actor in the update call 
	 * because we are in the middle of an iteration then.
	 * TODO: set property on actor that it is deleted so that we know
	 *       that in other parts of the engine
	 */
	void removeActor(EffActor* actor);

	/**
	 * To make an actor list which is up to date with deleted actors in
	 * the scene we tell the scene about the list, so that the scene can
	 * delete actors from this list too
	 */
	void addActorList (core::list<EffActor*>* list);

	/**
	 * Scene has a reference to the engine to do some special stuff
	 * TODO: we probably want to remove this reference
	 */
	void setEngine (EffEngine* engine);

	/**
	 * Sets the irrlicht scene manager to this scene, so that it is more
	 * easy to spawn new object
	 */
	void setManager (scene::ISceneManager* manager);

	// Get which keys are pressed
	// TODO: make special input class, done use the code from the engine
	InputReceiver* getInput(void);

	// Rerturns the recoreded delta mouse from the effengine
	core::vector2di getDeltaMouse (void);

	// Returns the current time from the irrlicht device
	const irr::u32 getTime(void);

	/**
	 * Cast ray to check for collision
	 * start, end -The start and end of the ray
	 * intersection -vector3df which is filled with the intersection of the collision
	 */
	scene::ISceneNode* checkRayCastIntersection(core::vector3df start, core::vector3df end, core::vector3df &intersection);

	/**
	 * Spawn mesh a certain position, only for debug purposes
	 */
	void spawnDebugMesh (core::vector3df position);
	
	/**
	 * Function to load a texture from the irrlicht driver object
	 */
	video::ITexture* getTexture (core::stringc pathname);

	~EffScene(void);

protected:

	// Change visibility of the mouse in the EffEngine
	// TODO: make special mouse object to do this, not in the effeninge class
	void setMouseVisible (bool mouseVisible);
	
	// The Irrlicht scene manager used to spawn object in the scene
	scene::ISceneManager* manager;
	ParticleManager* pManager;

	EffTimer* timer;

private:

	/**
	 * All the actors which were saved to be deleted will be deleted by calling
	 * this method. Usually this happens at the end of the frame
	 */
	void cleanupActors(void);
	bool removeActorFromList (EffActor* actorToRemove, core::list<EffActor*>* list);

	// TODO: remove the reference to the engine from the scene, I think
	EffEngine* engine;

	/**
	 * All actors in the scene
	 */
	core::list<EffActor*> actors;

	/**
	 * Actors which need to be removed at the end of the frame
	 */
	core::list<EffActor*> actorsToRemove;

	/**
	 * Other actor lists which are used in the game so that we can
	 * delete deleted actors from these lists too
	 */
	core::list<core::list<EffActor*>*> actorLists;

	bool isStopped;
};

#endif
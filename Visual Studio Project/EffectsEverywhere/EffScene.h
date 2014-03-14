#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <irrlicht.h>

using namespace irr;

class EffEngine;
class InputReceiver;
class EffActor;

class EffScene
{
public:
	EffScene();

	EffScene(EffEngine* engine);

	virtual bool init (void);

	virtual void update (float deltaTime);

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

	~EffScene(void);

protected:

	EffActor* addActor(EffActor* actor);

	// Function to load a texture from the irrlicht driver object
	video::ITexture* getTexture (core::stringc pathname);

	// Change visibility of the mouse in the EffEngine
	// TODO: make special mouse object to do this, not in the effeninge class
	void setMouseVisible (bool mouseVisible);

	// Rerturns the recoreded delta mouse from the effengine
	core::vector2di getDeltaMouse (void);

	// Get which keys are pressed
	// TODO: make special input class, done use the code from the engine
	InputReceiver* getInput(void);

	// Returns the current time from the irrlicht device
	const irr::u32 getTime(void);

	
	// The Irrlicht scene manager used to spawn object in the scene
	scene::ISceneManager* manager;

private:

	// TODO: remove the reference to the engine from the scene, I think
	EffEngine* engine;

	core::list<EffActor*> actors;
};

#endif
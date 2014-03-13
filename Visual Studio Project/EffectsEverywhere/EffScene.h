#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <irrlicht.h>

using namespace irr;

class EffEngine;
class InputReceiver;

class EffScene
{
public:
	EffScene();

	EffScene(EffEngine* engine);

	virtual bool init (void);

	virtual void update (float deltaTime);

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

	void setEngine (EffEngine* engine);

	~EffScene(void);
protected:
	scene::ISceneManager* manager;
	video::ITexture* getTexture (core::stringc pathname);
	void setMouseVisible (bool mouseVisible);
	InputReceiver* getInput(void);
	core::vector2di getDeltaMouse (void);
	const irr::u32 getTime(void);
private:
	EffEngine* engine;
};

#endif
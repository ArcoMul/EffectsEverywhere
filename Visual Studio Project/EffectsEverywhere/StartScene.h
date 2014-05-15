#ifndef STARTSCENE_H_
#define STARTSCENE_H_

#include <irrlicht.h>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include "EffScene.h"
#include <ParticleManager.h>
#include <ParticleModel.h>
#include <ParticleParser.h>

using namespace irr;

class EffEngine;
class Enemy;
class Robot;

class StartScene : public EffScene
{
public:
	StartScene();

	/**
	 * Creates all the objects, basically sets up the scene visually
	 */
	virtual bool init (void);
	
	virtual void update (float deltaTime);

	virtual void onButtonClick(s32 id);

	~StartScene(void);
	
private:

	/**
	 * The camera in the scene
	 */
	scene::ICameraSceneNode* camera;
};
enum
{
    GUI_ID_QUIT_BUTTON = 101,
    GUI_ID_START_BUTTON
};

#endif

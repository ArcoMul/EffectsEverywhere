#ifndef STARTSCENE_H_
#define STARTSCENE_H_

#include "EffScene.h"

using namespace irr;

class StartScene : public EffScene
{
public:
	StartScene();

	/**
	 * Creates all the objects, basically sets up the scene visually
	 */
	virtual bool init (void);
	
	/**
	 * When a button gets clicked in the scene
	 */
	virtual void onButtonClick(s32 id);

	~StartScene(void);

	enum BUTTONS
	{
		GUI_ID_START_BUTTON = 101,
		GUI_ID_INSTRUCTIONS_BUTTON,
		GUI_ID_QUIT_BUTTON
	};
	
private:

	/**
	 * The camera in the scene
	 */
	scene::ICameraSceneNode* camera;
};

#endif

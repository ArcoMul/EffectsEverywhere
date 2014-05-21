#ifndef INSTRUCTIONSSCENE_H_
#define INSTRUCTIONSSCENE_H_

#include "EffScene.h"

using namespace irr;

class InstructionsScene : public EffScene
{
public:
	InstructionsScene();

	/**
	 * Creates all the objects, basically sets up the scene visually
	 */
	virtual bool init (void);
	
	virtual void onButtonClick(s32 id);

	~InstructionsScene(void);

	enum BUTTONS
	{
		GUI_ID_BACK_BUTTON = 101
	};
	
private:

	/**
	 * The camera in the scene
	 */
	scene::ICameraSceneNode* camera;
};

#endif
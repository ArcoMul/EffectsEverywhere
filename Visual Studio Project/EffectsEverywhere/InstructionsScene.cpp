#include <iostream>
#include "InstructionsScene.h"
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "EffTimer.h"
#include "StartScene.h"

InstructionsScene::InstructionsScene()
{
}

bool InstructionsScene::init(void)
{
	EffScene::init ();

	// Show mouse in this scene
	setMouseVisible(true);
	
	// Background
	gui->addImage(this->getTexture("../../Media/menu/menu-help-background.png"), core::position2d<int>(0, 0));

	// Back button
	IGUIButton *backButton = gui->addButton(rect<s32>(80,470,251,527), 0, GUI_ID_BACK_BUTTON);
	backButton->setDrawBorder(false);
	backButton->setImage(getTexture("../../media/menu/button-back-default.png"));
	backButton->setPressedImage(getTexture("../../media/menu/button-back-active.png"));
	
	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	return true;
}


void InstructionsScene::onButtonClick(s32 id)
{
	switch(id)
    {
	case BUTTONS::GUI_ID_BACK_BUTTON:
		switchScene (new StartScene());
		break;
	default:
		break;
	}
}

InstructionsScene::~InstructionsScene(void)
{
}
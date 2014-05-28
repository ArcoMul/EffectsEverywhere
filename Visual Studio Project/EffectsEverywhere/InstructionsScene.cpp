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
	gui->addImage(this->getTexture(EffEngine::getPath("Media/menu/background.png").c_str()), core::position2d<int>(0, 0));

	gui->addImage(this->getTexture(EffEngine::getPath("Media/menu/menu-help.png").c_str()), core::position2d<int>(340, 135));

	// Back button
	IGUIButton *backButton = gui->addButton(rect<s32>(365,547,565,647), 0, GUI_ID_BACK_BUTTON);
	backButton->setDrawBorder(false);
	backButton->setImage(getTexture(EffEngine::getPath("Media/menu/button-back-default.png").c_str()));
	backButton->setPressedImage(getTexture(EffEngine::getPath("Media/menu/button-back-active.png").c_str()));
	
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
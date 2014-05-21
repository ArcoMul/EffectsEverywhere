#include <iostream>
#include "StartScene.h"
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "MainScene.h"
#include "InstructionsScene.h"

StartScene::StartScene()
{
	
}

bool StartScene::init(void)
{
	EffScene::init ();

	// Show the mouse in this scene
	setMouseVisible(true);
	
	// Background
	gui->addImage(getTexture("../../media/menu/background.png"),
		core::position2d<int>(0,0));

	// Start button
	IGUIButton *startButton = gui->addButton(rect<s32>(100,300,362,368), 0, GUI_ID_START_BUTTON);
	startButton->setDrawBorder(false);
	startButton->setImage(getTexture("../../media/menu/button-start-default.png"));
	startButton->setPressedImage(getTexture("../../media/menu/button-start-active.png"));

	// Help button
	IGUIButton *instructionsButton = gui->addButton(rect<s32>(100,400,362,468), 0, GUI_ID_INSTRUCTIONS_BUTTON);
	instructionsButton->setDrawBorder(false);
	instructionsButton->setImage(getTexture("../../media/menu/button-help-default.png"));
	instructionsButton->setPressedImage(getTexture("../../media/menu/button-help-active.png"));

	// Exit button
	IGUIButton *quitButton = gui->addButton(rect<s32>(100,500,362,568), 0, GUI_ID_QUIT_BUTTON);
	quitButton->setDrawBorder(false);
	quitButton->setImage(getTexture("../../media/menu/button-exit-default.png"));
	quitButton->setPressedImage(getTexture("../../media/menu/button-exit-active.png"));
	
	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	return true;
}

void StartScene::onButtonClick(s32 id)
{
	switch(id)
    {
	case BUTTONS::GUI_ID_START_BUTTON:
		switchScene (new MainScene());
		break;
	case BUTTONS::GUI_ID_INSTRUCTIONS_BUTTON:
		switchScene (new InstructionsScene());
		break;
    case BUTTONS::GUI_ID_QUIT_BUTTON:
		closeGame();
		break;
	default:
		break;
	}
	
}

StartScene::~StartScene(void)
{
	
}
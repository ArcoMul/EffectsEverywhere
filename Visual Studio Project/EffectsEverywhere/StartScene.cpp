#include <iostream>
#include "StartScene.h"
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "MainScene.h"
#include "InstructionsScene.h"
#include "GuiAnimation.h"

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
	startButton = gui->addButton(rect<s32>(-262,300,0,368), 0, GUI_ID_START_BUTTON);
	startButton->setDrawBorder(false);
	startButton->setImage(getTexture("../../media/menu/button-start-default.png"));
	startButton->setPressedImage(getTexture("../../media/menu/button-start-active.png"));
	startButtonAnim = new GuiAnimation(startButton, 100, 300, 1.1, 200);
	
	// Help button
	instructionsButton = gui->addButton(rect<s32>(-262,400,0,468), 0, GUI_ID_INSTRUCTIONS_BUTTON);
	instructionsButton->setDrawBorder(false);
	instructionsButton->setImage(getTexture("../../media/menu/button-help-default.png"));
	instructionsButton->setPressedImage(getTexture("../../media/menu/button-help-active.png"));
	instructionsButtonAnim = new GuiAnimation(instructionsButton, 100, 400, 1.30, 300);
	
	// Exit button
	quitButton = gui->addButton(rect<s32>(-262,500,0,568), 0, GUI_ID_QUIT_BUTTON);
	quitButton->setDrawBorder(false);
	quitButton->setImage(getTexture("../../media/menu/button-exit-default.png"));
	quitButton->setPressedImage(getTexture("../../media/menu/button-exit-active.png"));
	quitButtonAnim = new GuiAnimation(quitButton, 100, 500, 1.50, 400);
	
	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	return true;
}

void StartScene::update(float deltaTime)
{
	startButtonAnim->update(deltaTime);
	instructionsButtonAnim->update(deltaTime);
	quitButtonAnim->update(deltaTime);
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
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

	this->getSoundEngine()->stopAllSounds();
	this->getSoundEngine()->play2D("../../Media/sounds/ipgamesopenloop.wav",true);
	
	// Background
	gui->addImage(getTexture("../../media/menu/background.png"), core::position2d<int>(0,0));
	gui->addImage(getTexture("../../media/menu/logo.png"), core::position2d<int>(55,45));

	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	// Robot
	gui::IGUIImage* robot = gui->addImage(getTexture("../../media/menu/robot.png"), core::position2d<int>(760,180));
	robotAnim = new GuiAnimation(robot, 760, 150, 0.015, 500, true);
	gui->addImage(getTexture("../../media/menu/glow-floor.png"), core::position2d<int>(770,650));

	// Start button
	startButton = gui->addButton(rect<s32>(-262,432,0,500), 0, GUI_ID_START_BUTTON);
	startButton->setDrawBorder(false);
	startButton->setImage(getTexture("../../media/menu/button-start-default.png"));
	startButton->setPressedImage(getTexture("../../media/menu/button-start-active.png"));
	startButtonAnim = new GuiAnimation(startButton, 100, 432, 1.1, 200);
	
	// Help button
	instructionsButton = gui->addButton(rect<s32>(-262,532,0,600), 0, GUI_ID_INSTRUCTIONS_BUTTON);
	instructionsButton->setDrawBorder(false);
	instructionsButton->setImage(getTexture("../../media/menu/button-help-default.png"));
	instructionsButton->setPressedImage(getTexture("../../media/menu/button-help-active.png"));
	instructionsButtonAnim = new GuiAnimation(instructionsButton, 100, 532, 1.30, 300);
	
	// Exit button
	quitButton = gui->addButton(rect<s32>(-262,632,0,700), 0, GUI_ID_QUIT_BUTTON);
	quitButton->setDrawBorder(false);
	quitButton->setImage(getTexture("../../media/menu/button-exit-default.png"));
	quitButton->setPressedImage(getTexture("../../media/menu/button-exit-active.png"));
	quitButtonAnim = new GuiAnimation(quitButton, 100, 632, 1.50, 400);
	
	return true;
}

void StartScene::update(float deltaTime)
{
	startButtonAnim->update(deltaTime);
	instructionsButtonAnim->update(deltaTime);
	quitButtonAnim->update(deltaTime);
	robotAnim->update(deltaTime);
}

void StartScene::onButtonClick(s32 id)
{
	this->getSoundEngine()->play2D("../../Media/sounds/button.wav",false);
	switch(id)
    {
	case BUTTONS::GUI_ID_START_BUTTON:
		switchScene (new MainScene());
		break;
	case BUTTONS::GUI_ID_INSTRUCTIONS_BUTTON:
		switchScene (new InstructionsScene());
		break;
    case BUTTONS::GUI_ID_QUIT_BUTTON:
		this->getSoundEngine()->stopAllSounds();
		closeGame();
		break;
	default:
		break;
	}
	
}

StartScene::~StartScene(void)
{
	
}
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
	gui->addImage(getTexture(EffEngine::getPath("media/menu/background.png").c_str()), core::position2d<int>(0,0));
	gui->addImage(getTexture(EffEngine::getPath("media/menu/logo.png").c_str()), core::position2d<int>(55,45));

	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	// Robot
	gui::IGUIImage* robot = gui->addImage(getTexture(EffEngine::getPath("Media/menu/robot.png").c_str()), core::position2d<int>(760,180));
	robotAnim = new GuiAnimation(robot, 760, 150, 0.015, 500, true);
	gui->addImage(getTexture(EffEngine::getPath("Media/menu/glow-floor.png").c_str()), core::position2d<int>(770,650));

	// Start button
	startButton = gui->addButton(rect<s32>(-262,432,0,500), 0, GUI_ID_START_BUTTON);
	startButton->setDrawBorder(false);
	startButton->setImage(getTexture(EffEngine::getPath("Media/menu/button-start-default.png").c_str()));
	startButton->setPressedImage(getTexture(EffEngine::getPath("Media/menu/button-start-active.png").c_str()));
	startButtonAnim = new GuiAnimation(startButton, 100, 432, 1.1, 200);
	
	// Help button
	instructionsButton = gui->addButton(rect<s32>(-262,532,0,600), 0, GUI_ID_INSTRUCTIONS_BUTTON);
	instructionsButton->setDrawBorder(false);
	instructionsButton->setImage(getTexture(EffEngine::getPath("Media/menu/button-help-default.png").c_str()));
	instructionsButton->setPressedImage(getTexture(EffEngine::getPath("Media/menu/button-help-active.png").c_str()));
	instructionsButtonAnim = new GuiAnimation(instructionsButton, 100, 532, 1.30, 300);
	
	// Exit button
	quitButton = gui->addButton(rect<s32>(-262,632,0,700), 0, GUI_ID_QUIT_BUTTON);
	quitButton->setDrawBorder(false);
	quitButton->setImage(getTexture(EffEngine::getPath("Media/menu/button-exit-default.png").c_str()));
	quitButton->setPressedImage(getTexture(EffEngine::getPath("Media/menu/button-exit-active.png").c_str()));
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
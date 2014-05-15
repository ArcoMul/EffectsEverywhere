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

	camera = manager->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0));

	// Set mouse Visible to false
	setMouseVisible(true);
	
	gui->addImage(getTexture("../../media/menu/background.png"),
		core::position2d<int>(0,0));

	IGUIButton *startButton = gui->addButton(rect<s32>(100,300,362,368), 0, GUI_ID_START_BUTTON);
	startButton->setDrawBorder(false);
	startButton->setImage(getTexture("../../media/menu/button-start-default.png"));
	startButton->setPressedImage(getTexture("../../media/menu/button-start-active.png"));

	IGUIButton *instructionsButton = gui->addButton(rect<s32>(100,400,362,468), 0, GUI_ID_INSTRUCTIONS_BUTTON);
	instructionsButton->setDrawBorder(false);
	instructionsButton->setImage(getTexture("../../media/menu/button-help-default.png"));
	instructionsButton->setPressedImage(getTexture("../../media/menu/button-help-active.png"));

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
	case GUI_ID_START_BUTTON:
        std::cout << "Start" << std::endl;
		switchScene (new MainScene());
		break;
	case GUI_ID_INSTRUCTIONS_BUTTON:
        std::cout << "Instructions" << std::endl;
		switchScene (new InstructionsScene());
		break;
    case GUI_ID_QUIT_BUTTON:
        std::cout << "Quit" << std::endl;
		closeGame();
		break;
	default:
		break;
	}
	
}
void StartScene::update(float deltaTime)
{
	EffScene::update(deltaTime);	
}

StartScene::~StartScene(void)
{
	
}
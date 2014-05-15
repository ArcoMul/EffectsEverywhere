#include <iostream>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include "StartScene.h"
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "EffTimer.h"
#include "MainScene.h"
#include "TemporaryParticleEffect.h"

StartScene::StartScene()
{
	
}

bool StartScene::init(void)
{
	EffScene::init ();
	
	
	// Set a jump of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	camera = manager->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0));

	// Set mouse Visible to false
	setMouseVisible(true);
	
	IGUISkin* skin = gui->getSkin();
	IGUIFont* font = gui->getFont("../../media/fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);
	skin->setFont(gui->getBuiltInFont(), EGDF_TOOLTIP);
	
	gui->addStaticText(L"Yolo Swaggerino:", rect<s32>(150,20,350,40), false);

	//gui->addImage(this->getTexture("../../Media/irrlichtlogo2.png"),
	//	core::position2d<int>(this->getDriverWidth()- 200, this->getDriverHeight()-200));

	IGUIButton *startButton = gui->addButton(rect<s32>(300,300,500,375), 0, GUI_ID_START_BUTTON,
            L"Start");
	
	//ITexture *startButton1 = getTexture("../../media/stones.jpg");
	startButton->setDrawBorder(false);
	startButton->setImage(getTexture("../../media/stones.jpg"));
	startButton->setPressedImage(getTexture("../../media/water.jpg"));
	
	//gui->addButton(rect<s32>(300,300,500,350), 0, GUI_ID_START_BUTTON,
    //        L"Start");
	gui->addButton(rect<s32>(300,400,500,450));
	gui->addButton(rect<s32>(300,500,500,550), 0, GUI_ID_QUIT_BUTTON,
            L"Quit");

	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	// Set the camera position and rotation plus
	// the camera follows the robot.
	//camera->setPosition(vector3df(0, 40, 80));
	//camera->setRotation(vector3df(0, 180, 0));

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
	// When the spacebar is pressed and the cooldown is low enough, shoot!
	if (getInput()->IsKeyDown(irr::KEY_SPACE))
	{
		TemporaryParticleEffect* p = new TemporaryParticleEffect(800);
		this->addXMLParticleActor((EffActor*) p, "../../Media/Start.xml", core::vector3df(50, 0 , 180));
	}
	
}

StartScene::~StartScene(void)
{
	
}
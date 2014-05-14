#include <iostream>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include "StartScene.h"
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "EffTimer.h"
#include "MainScene.h"

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
	
	
	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	// Set the camera position and rotation plus
	// the camera follows the robot.
	//camera->setPosition(vector3df(0, 40, 80));
	//camera->setRotation(vector3df(0, 180, 0));

	return true;
}



void StartScene::update(float deltaTime)
{
	EffScene::update(deltaTime);
	// When the spacebar is pressed and the cooldown is low enough, shoot!
	if (getInput()->IsKeyDown(irr::KEY_SPACE))
	{
		std::cout << "Start" << std::endl;
		switchScene (new MainScene());
	}
}

StartScene::~StartScene(void)
{
	
}
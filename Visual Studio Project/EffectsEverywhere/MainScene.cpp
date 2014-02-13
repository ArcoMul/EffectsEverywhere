#include <iostream>
#include "MainScene.h"
#include "GameEngine.h"
#include "InputReceiver.h"
#include "BackgroundFader.h"

MainScene::MainScene(GameEngine* engine) :
	_engine(engine)
{
	this->backgroundFader = new BackgroundFader(engine);
}

void MainScene::start(void)
{
	// The the mesh from the system
	IMesh* mesh = _engine->smgr->getMesh("../../Media/robot.obj");

	// Add a new Irrlicht Node with the loaded mesh as mesh
	robot = _engine->smgr->addMeshSceneNode(mesh);

	// Make sure the node is loaded and set what kind of matarial it is
	if (robot) {
		robot->setMaterialFlag(EMF_LIGHTING, false);
	}

	// Add the camera node to the scene
	_engine->smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));
}

void MainScene::update(void)
{
	// Get the position of the robot
	core::vector3df pos = robot->getPosition();

	// Get the rotation of the robot
	core::vector3df rot = robot->getRotation();

	// Get the transformations done on this robot
	core::matrix4 mat = robot->getAbsoluteTransformation();

	// When the W key is down
	if(_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_W))
	{
		// Multiply the already done transformations of the robot with the speed and deltaTime
		pos += core::vector3df(mat[2] * .1 * _engine->deltaTime,
			0,
			mat[0] * -.1 * _engine->deltaTime);
	}
	// When the S key is down
	else if(_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_S))
	{
		// Same as above, but with inverse Z speed
		pos += core::vector3df(mat[2] * .1 * _engine->deltaTime,
			0,
			mat[0] * .1 * _engine->deltaTime);
	}

	// Rotate on the Y angle when the A or D key is down
	if(_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_A))
		rot.Y -= .4 * _engine->deltaTime;
	else if(_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_D))
		rot.Y += .4 * _engine->deltaTime;

	// Set the newly calculated position and rotation
	robot->setPosition(pos);
	robot->setRotation(rot);

	// Calculate the new colors for the background fader
	backgroundFader->fade();

	// Set the background fader color to the engine background color
	_engine->backgroundColor = backgroundFader->getColor();
}

MainScene::~MainScene(void)
{
	delete backgroundFader;
}

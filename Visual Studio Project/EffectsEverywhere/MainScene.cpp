#include <iostream>
#include "MainScene.h"
#include "GameEngine.h"
#include "InputReceiver.h"
#include "BackgroundFader.h"
#include "Enemy.h"

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

	// Make sure the node is loaded and 
	if (robot)
	{
		// Set what kind of matarial it is

		robot->setMaterialFlag(EMF_LIGHTING, false);

		// Set start position (on top of floor)
		robot->setPosition(core::vector3df(0, 7.2f, 0));
	}

	// Add floor to scene
	IMesh* floorMesh = _engine->smgr->getMesh("../../Media/floor.obj");
	IMeshSceneNode* floor = _engine->smgr->addMeshSceneNode(floorMesh);
	if (floor) {
		floor->setMaterialFlag(EMF_LIGHTING, false);
	}

	//Set a jump of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	camera = _engine->smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, -1, 0, 0, true, 3.f);

	// Creating an enemy and give it the parameters from the Enemy.cpp class
	Enemy* enemy1 = new Enemy(_engine, core::vector3df(0, 0, -55));
	enemy1->addCollision(robot);

	// Set the camera position and rotation plus
	// the camera follows the robot.
	camera = _engine->smgr->addCameraSceneNode();
	camera->setPosition(vector3df(0, 30, 40));
	camera->setRotation(vector3df(0, 180, 0));
	robot->addChild(camera);
 
}

void MainScene::update(void)
{
	// Get the position of the robot
	core::vector3df pos = robot->getPosition();

	// Get the rotation of the robot
	core::vector3df rot = robot->getRotation();
	
	// Set camera position update
	camera->setTarget(pos);

	// Get the transformations done on this robot
	core::matrix4 mat = robot->getAbsoluteTransformation();

	// Movement speed
	float speed = .1;

	// WARNING HACKY: When both a front/back key and a right/left key is pressed reduce the speed,
	// so that it doesn't move twice as fast when going in a diagonal line
	if ((_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_W) || _engine->inputReceiver->IsKeyDown(irr::KEY_KEY_S))
		&& (_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_A) || _engine->inputReceiver->IsKeyDown(irr::KEY_KEY_D)))
		speed *= 0.667; // 0.667 is sort of the factor of the distance you move when you go in a 45 degree angle


	// When the W key is down
	if(_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_W))
	{
		// Multiply the already done transformations of the robot with the speed and deltaTime
		pos += core::vector3df(mat[2] * speed * _engine->deltaTime,
			0,
			mat[0] * -speed * _engine->deltaTime);
	}
	// When the S key is down go back
	else if(_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_S))
	{
		pos += core::vector3df(mat[2] * -speed * _engine->deltaTime,
			0,
			mat[0] * speed * _engine->deltaTime);
	}
	
	// When the A key is down go right
	if(_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_A))
	{
		pos += core::vector3df(mat[0] * speed * _engine->deltaTime,
			0,
			mat[2] * speed * _engine->deltaTime);
	}
	// When the D key is down go left
	else if(_engine->inputReceiver->IsKeyDown(irr::KEY_KEY_D))
	{
		pos += core::vector3df(mat[0] * -speed * _engine->deltaTime,
			0,
			mat[2] * -speed * _engine->deltaTime);
	}

	// Set the newly calculated position and rotation
	robot->setPosition(pos);
	robot->setRotation(rot);

	// Calculate the new colors for the background fader
	backgroundFader->fade();
}

MainScene::~MainScene(void)
{
	delete backgroundFader;
}

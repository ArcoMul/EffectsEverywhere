#include <iostream>
#include "MainScene.h"
#include "GameEngine.h"
#include "InputReceiver.h"
#include "Enemy.h"

MainScene::MainScene(GameEngine* engine) :
	_engine(engine)	
{
	// Set the whole bullet array at NULL so that we can check if the item is NULL or not later on
	for (int i = 0; i < 10; i++) {
		bullets[i] = NULL;
	}

	// Active bullet index starts at zero
	bulletIndex = 0;
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
	IMesh* floorMesh = _engine->smgr->getMesh("../../Media/level.obj");
	IMeshSceneNode* floor = _engine->smgr->addMeshSceneNode(floorMesh);
	if (floor) {
		floor->setMaterialFlag(EMF_LIGHTING, false);
	}

	//Set a jump of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	camera = _engine->smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, -1, 0, 0, true, 3.f);

	// Creating an enemy and give it the parameters from the Enemy.cpp class
	Enemy* enemy1 = new Enemy(_engine, core::vector3df(-30, 0, -55));
	Enemy* enemy2 = new Enemy(_engine, core::vector3df(30, 0, -55));

	// Add collision with the player and the enemies
	enemy1->addCollision(robot);
	enemy2->addCollision(robot);

	// Set the camera position and rotation plus
	// the camera follows the robot.

	// Set mouse Visible to false
	_engine->setMouseVisible(false);
	
	// Add the camera node to the scene

	camera = _engine->smgr->addCameraSceneNode();
	camera->setPosition(vector3df(0, 40, 55));
	camera->setRotation(vector3df(0, 180, 0));
	robot->addChild(camera);

	_engine->backgroundColor = video::SColor(1, 0, 0, 0);
}

void MainScene::update(void)
{
	// Get the position of the robot
	core::vector3df pos = robot->getPosition();

	// Get the rotation of the robot
	core::vector3df rot = robot->getRotation();

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

	// Add deltaMouse, the change of mouse position, to the rotation of the robot
    rot.Y += -.3 * _engine->deltaMouse.X;

	// Set the newly calculated position and rotation
	robot->setPosition(pos);
	robot->setRotation(rot);

	// Set where the camera has to look at
	camera->setTarget(robot->getPosition());

	// Reduce the cooldown of shooting
	if (shootCooldown > 0) {
		shootCooldown -= _engine->deltaTime;
	}

	// When the spacebar is pressed and the cooldown is low engouh, shoot!
	if (_engine->inputReceiver->IsKeyDown(irr::KEY_SPACE) && shootCooldown <= 0) {

		// Reset the cooldown
		shootCooldown = 250;

		// Create the bullet mesh
		IMesh* bulletMesh = _engine->smgr->getMesh("../../Media/bullet.obj");
		IMeshSceneNode* bulletNode = _engine->smgr->addMeshSceneNode(bulletMesh);

		// Put the mesh in a bullet class together with the time the bullet was created
		Bullet* bullet = new Bullet(bulletNode, _engine->totalTime);

		// Give the bullet mesh the right material and position
		bullet->node->setMaterialFlag(EMF_LIGHTING, false);
		bullet->node->setPosition(robot->getPosition());
		bullet->node->setRotation(robot->getRotation());

		// If there is already a bullet in the bullet array on this position, remove that bullet
		if (bullets[bulletIndex] != NULL) {
			bullets[bulletIndex]->node->remove();
			delete bullets[bulletIndex];
			bullets[bulletIndex] = NULL;
		}

		// Add the bullet to the bullet array on the active index
		bullets[bulletIndex] = bullet;

		// Add one to the active bullet index or reset it to zero
		if (bulletIndex < 9) {
			bulletIndex++;
		} else {
			bulletIndex = 0;
		}
	}

	// Update all the bullets
	for (int i = 0; i < 10; i++)
	{
		if (bullets[i] == NULL) continue;

		// Remove the bullet if it is alive for 500 miliseconds
		if (_engine->totalTime - bullets[i]->aliveSince > 500)
		{
			bullets[i]->node->remove();
			delete bullets[i];
			bullets[i] = NULL;
			continue;
		}

		// Get the current position and rotation and calculate based on that the new position
		core::vector3df pos = bullets[i]->node->getPosition();
		core::matrix4 mat = bullets[i]->node->getAbsoluteTransformation();
		pos += core::vector3df(mat[2] * .5 * _engine->deltaTime,
			0,
			mat[0] * -.5 * _engine->deltaTime);
		bullets[i]->node->setPosition(pos);
	}
}

MainScene::~MainScene(void)
{
	
}

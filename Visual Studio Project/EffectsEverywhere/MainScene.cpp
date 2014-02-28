#include <iostream>
#include "MainScene.h"
#include "GameEngine.h"
#include "InputReceiver.h"
#include "BackgroundFader.h"


enum
{
	//Indicatior that a scene node is not pickable
	//by getSceneNodeAndCollisionPointFromRay().
	ID_IsNotPickable = 0,

	//Indicator that a scene note is pickable
	//by ray selection.
	IDFlag_IsPickable = 1 << 0,

	//Indicator that the scene node can be highlited.
	IDFlagIsHighlightable = 1 << 1
};

MainScene::MainScene(GameEngine* engine) :
	_engine(engine)
	
{
	this->backgroundFader = new BackgroundFader(engine);
}

void MainScene::start(void)
{
	// The the mesh from the system
	IMesh* mesh = _engine->smgr->getMesh("../../Media/robot.obj");

	//Load 2 enemies into the world
	IAnimatedMesh* meshEnemy1 = _engine->smgr->getMesh("../../Media/Wall.obj");
	IAnimatedMesh* meshEnemy2 = _engine->smgr->getMesh("../../Media/Wall.obj");

	// Add a new Irrlicht Node with the loaded mesh as mesh
	robot = _engine->smgr->addMeshSceneNode(mesh);

	// Make sure the node is loaded and 
	if (robot)
	{
		// Set what kind of matarial it is

		robot->setMaterialFlag(EMF_LIGHTING, false);

		// Set start position (on top of floor)
		robot->setPosition(core::vector3df(0, 7.2, 0));
	}

	// Add floor to scene
	IMesh* floorMesh = _engine->smgr->getMesh("../../Media/floor.obj");
	IMeshSceneNode* floor = _engine->smgr->addMeshSceneNode(floorMesh);
	if (floor) {
		floor->setMaterialFlag(EMF_LIGHTING, false);
	}

	//The cube mesh is pickable, but doesn't get highlighted.
	if (meshEnemy1){
		enemy1 = _engine->smgr->addOctreeSceneNode(meshEnemy1, 0, IDFlag_IsPickable);
	}

	// The enemy2 is pickable, but doesn't get highlighted.
	if (meshEnemy2) {
		enemy2 = _engine->smgr->addOctreeSceneNode(meshEnemy2, 0, IDFlag_IsPickable);
	}

	//Creating a triangle selector
	if (enemy1){
		selectorEnemy1 = _engine->smgr->createOctreeTriangleSelector(
			enemy1->getMesh(), enemy1, 12);
		enemy1->setTriangleSelector(selectorEnemy1);
		enemy1->setPosition(core::vector3df(0, 10, -55));
	}

	//Creating a second triangle selector, this time for the second enemy.
	if (enemy2) {
		selectorEnemy2 = _engine->smgr->createOctreeTriangleSelector(
			enemy2->getMesh(), enemy2, 12);
		enemy2->setTriangleSelector(selectorEnemy2);
		enemy2->setPosition(core::vector3df(30, 10, -55));
	}

	//Set a jump of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	camera = _engine->smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);

	//Check for collision and give an animator to the robot when it collides with something
	if(selectorEnemy1) {

		collision = _engine->smgr->createCollisionResponseAnimator(
			selectorEnemy1, camera, core::vector3df(7, 7, 7),
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 1));

		scene::ISceneNodeAnimator* anim = (scene::ISceneNodeAnimator*) collision;
		selectorEnemy1->drop();
		robot->addAnimator(anim);
		anim->drop();
	}

	//Collision check for enemy2.
		if(selectorEnemy2) {

		collision2 = _engine->smgr->createCollisionResponseAnimator(
			selectorEnemy2, camera, core::vector3df(7, 7, 7),
			core::vector3df(0, 0, 0), core::vector3df(0, 0, 1));

		scene::ISceneNodeAnimator* anim2 = (scene::ISceneNodeAnimator*) collision2;
		selectorEnemy2->drop();
		robot->addAnimator(anim2);
		anim2->drop();
	}

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

	// Ouput collision when there is collision in the last frame
	if (collision->collisionOccurred()) {
		std::cout << "Collision!" << std::endl;
	}

	if (collision2->collisionOccurred()) {
		std::cout << "Collision with enemy2!!" << std::endl;
	}
}

MainScene::~MainScene(void)
{
	delete backgroundFader;
}

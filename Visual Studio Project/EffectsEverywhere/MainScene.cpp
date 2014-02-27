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

scene::ICameraSceneNode* camera;
scene::ISceneCollisionManager* collMan;

MainScene::MainScene(GameEngine* engine) :
	_engine(engine)
	
{
	this->backgroundFader = new BackgroundFader(engine);
}

void MainScene::start(void)
{
	// The the mesh from the system
	IMesh* mesh = _engine->smgr->getMesh("../../Media/robot.obj");

	//Load a cube into the world
	scene::IAnimatedMesh* meshCube = _engine->smgr->getMesh("../../Media/Wall.obj");

	// Add a new Irrlicht Node with the loaded mesh as mesh
	robot = _engine->smgr->addMeshSceneNode(mesh);
	
	//Add a node and set it to null
	scene::IMeshSceneNode* cube = 0;


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

	
	
	// Add the camera node to the scene

	_engine->smgr->addCameraSceneNode(0, vector3df(10, 30, -50),vector3df(0, 5, 0));



	//The cube mesh is pickable, but doesn't get highlighted.
	if(meshCube){
		cube = _engine->smgr->addOctreeSceneNode(meshCube, 0, IDFlag_IsPickable);
	}

	//Creating a triangle selector
	scene::ITriangleSelector* selector = 0;

	if(cube){
		selector = _engine->smgr->createOctreeTriangleSelector(
			cube->getMesh(), cube, 12);
		cube->setTriangleSelector(selector);
		cube->setPosition(core::vector3df(0, 10, -55));
	}

	//Set a jump of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	camera = _engine->smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
	//camera->setPosition(core::vector3df(0, 30, 20));
	//camera->setTarget(core::vector3df(0, 0, 0));
	
	if(selector) {
		scene::ISceneNodeAnimator* anim = _engine->smgr->createCollisionResponseAnimator(
			selector, camera, core::vector3df(30, 50, 30),
			core::vector3df(0, 0, 0), core::vector3df(0, 30, 0));
		selector->drop();
		camera->addAnimator(anim);
		anim->drop();
	}

	collMan = _engine->smgr->getSceneCollisionManager();
	int lastFPS = -1;


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

	// Set the background fader color to the engine background color
	_engine->backgroundColor = backgroundFader->getColor();

	// Always check if there is collision
	core::line3d<f32> ray;
	ray.start = camera->getPosition();
	ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

	// Tracks the current intersection point with the level or mesh
	core::vector3df intersection;

	// Used to show which triangle has been hit
	core::triangle3df hitTriangle;

	// This call is nessecary to do the triangle collision on every node
	// that has a triangle selector. It will find the nearest collision point and returns the scene node
	// containing that point.
	scene::ISceneNode * selectedSceneNode =
		collMan->getSceneNodeAndCollisionPointFromRay(
			ray,
			intersection,
			hitTriangle,
			IDFlag_IsPickable,
			0);
}

MainScene::~MainScene(void)
{
	delete backgroundFader;
}

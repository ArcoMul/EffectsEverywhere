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

	// Make sure the nodes are loaded and set what kind of matarial it is
	if (robot) {
		robot->setMaterialFlag(EMF_LIGHTING, false);
	}

	// Add the camera node to the scene
	//_engine->smgr->addCameraSceneNode(0, vector3df(10, 30, -50),vector3df(0, 5, 0));

	//The cube mesh is pickable, but doesn't get highlighted.
	if(meshCube){
		cube = _engine->smgr->addOctreeSceneNode(meshCube, 0, IDFlag_IsPickable);
	}

	//Creating a triangle selector
	scene::ITriangleSelector* selector = 0;

	if(cube){
		selector = _engine->smgr->createOctreeTriangleSelector(
			cube->getMesh(), cube, 128);
		cube->setTriangleSelector(selector);
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

}

void MainScene::update(void)
{
	// Get the position of the robot
	core::vector3df pos = robot->getPosition();

	// Get the rotation of the robot
	core::vector3df rot = robot->getRotation();

	// Get the transformations done on this robot
	core::matrix4 mat = robot->getAbsoluteTransformation();

	//Set camera to robot
	camera->setPosition(vector3df(pos.X, pos.Y+30, pos.Z + 40));
	//camera->setPosition(pos);
	camera->setTarget(pos);

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

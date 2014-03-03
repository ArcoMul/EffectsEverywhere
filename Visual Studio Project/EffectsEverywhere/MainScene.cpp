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

	//creating a particlesystemscenenode which basicly is a particle
	IParticleSystemSceneNode* ps = _engine->smgr->addParticleSystemSceneNode(false);
	
	//creating an emitter so u actually emit the particle from somewhere so it will be visual( in this case it's a box )
	IParticleEmitter* em = ps->createBoxEmitter(aabbox3df(-5, 0, -5, 5, 1, 5 ),vector3df(0.0f, 0.1f, 0.0f),50,200,SColor(0, 0, 0, 255),
		SColor(0,255,255,255),800,1000,0,dimension2df(10.0f, 10.0f), dimension2df(20.0f, 20.0f));

	// adding a particle affector which causes the particles to fade out
	IParticleAffector* paf = ps->createFadeOutParticleAffector();

    ps->addAffector(paf); // same goes for the affector
    paf->drop();

	//add the emitter to the particle and drop to prevent memory leakage
	ps->setEmitter(em);
	em->drop();

	//check if the particlesystemscenenode is created correctly
	if(ps){
		ps->setPosition(vector3df(-20, -15, -80));
		ps->setScale(vector3df(2,2,2));
		ps->setMaterialFlag(EMF_LIGHTING, false);
		ps->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		ps->setMaterialTexture(0, _engine->driver->getTexture("../../Media/fireball.bmp"));
		ps->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	}

	// create a hilleplanemesh to simulate height so we can create waves for the water particle
	 IMesh* watermesh = _engine->smgr->addHillPlaneMesh("watermesh",dimension2d<f32>(20, 20),dimension2d<u32>(20,20),0,0,dimension2d<f32>(0,0),dimension2d<f32>(10,10));
	 ISceneNode* waternode = _engine->smgr->addWaterSurfaceSceneNode(_engine->smgr->getMesh("watermesh"),3.0f,300.0f,30.0f);

	 if(waternode){
		 waternode->setPosition(vector3df(0,5,0));
		 waternode->setMaterialTexture(0, _engine->driver->getTexture("../../Media/stones.jpg"));
		 waternode->setMaterialTexture(1, _engine->driver->getTexture("../../Media/water.jpg"));
		 waternode->setMaterialType(EMT_REFLECTION_2_LAYER);
		 waternode->setMaterialFlag(EMF_LIGHTING,false);
	 }

	// Add the camera node to the scene
	camera = _engine->smgr->addCameraSceneNode();
	camera->setPosition(vector3df(0, 30, 80));
	camera->setRotation(vector3df(0, 180, 0));
	robot->addChild(camera);
	robot->addChild(ps);

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
}

MainScene::~MainScene(void)
{
	delete backgroundFader;
}

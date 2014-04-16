#include <iostream>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include "MainScene.h"
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Robot.h"
#include "TemporaryParticleEffect.h"
#include <ParticleManager.h>
#include <ParticleModel.h>

MainScene::MainScene()
{
}

bool MainScene::init(void)
{
	EffScene::init ();
	
	// Hit effect on the robot it is passed to the robot and then to the bullet
	ParticleModel* pModel = new ParticleModel();
	pModel->setEmitterType(ParticleModel::EmitterTypes::POINT);
	pModel->setMinColor(video::SColor(0,180,180,180));
	pModel->setMaxColor(video::SColor(0, 255, 255, 255));
	pModel->setMinPPS(50);
	pModel->setMaxPPS(200);
	pModel->setAabbox(core::aabbox3df(-3, 0, -3, 3, 1, 3 ));
	pModel->setDirection(core::vector3df(0.02f, 0.0f, 0.0f));
	pModel->setLifeTimeMax(700);
	pModel->setLifeTimeMin(500);
	pModel->setMaxAngleDegrees(360);
	pModel->setMinStartSize(core::dimension2df(4.5f, 4.5f));
	pModel->setMaxStartSize(core::dimension2df(9.0f, 9.0f));
	pModel->setPathNameTexture("../../Media/smoke.png");
	pModel->setPosition(core::vector3df(2,2,2));

	// Create robot actor
	robot = new Robot ();
	addNodeActor ((EffActor*) robot, core::vector3df(0, 7.5f, 0), core::vector3df(0, 0, 0));
	if (!robot) return false;

	// add Gun & Bullet
	robot->setWeapon("../../Media/rock-gun.obj","../../Media/rock-bullet.obj",3,0.6,250,pModel,pModel,pModel);
	
	// Add floor to scene
	scene::IMesh* floorMesh = manager->getMesh("../../Media/level.obj");
	scene::IMeshSceneNode* floor = manager->addMeshSceneNode(floorMesh);
	if (!floor) return false;
	floor->setMaterialFlag(EMF_LIGHTING, false);

	// WARNING: we are just spawning a really big particle field,
	// this is because we cant spawn less than 1 particle per second
	// a really big particle effect has the same effect though
	ParticleModel* levelParticles = new ParticleModel();
	levelParticles->setEmitterType(ParticleModel::EmitterTypes::BOX);
	levelParticles->setMinColor(video::SColor(0, 255, 255, 255));
	levelParticles->setMaxColor(video::SColor(0, 255, 210, 0));
	levelParticles->setMinPPS(0);
	levelParticles->setMaxPPS(1);
	levelParticles->setAabbox(core::aabbox3df(-5000, 0, -5000, 5000, 1, 5000 ));
	levelParticles->setDirection(core::vector3df(0.0f, 0.01f, 0.0f));
	levelParticles->setLifeTimeMax(10000);
	levelParticles->setLifeTimeMin(5000);
	levelParticles->setMaxAngleDegrees(0);
	levelParticles->setMinStartSize(core::dimension2df(1.0f, 1.0f));
	levelParticles->setMaxStartSize(core::dimension2df(6.0f, 6.0f));
	levelParticles->setPathNameTexture("../../Media/portal1.bmp");
	levelParticles->setPosition(core::vector3df(4,2,2));
	pManager->spawnDataModelParticle(levelParticles ,levelParticles->getPosition() ,levelParticles->getPathNameTexture());

	// Create a Triangle selector for the level
	scene::ITriangleSelector* levelSelector = manager->createOctreeTriangleSelector(floor->getMesh(), floor, 12);
	floor->setTriangleSelector(levelSelector);

	// Add an animator to the camera, a Collision Response Animator. This animator prevents
	// your object (player) to move through walls and other objects. The collision box of the enemy
	// has been set to 7, 7, 10. We do nothing with the gravity, this is why we set the vector to 0, 0, 0.
	// The last vector is a translation for the animator, which is set to 0, 0, 1.
	scene::ISceneNodeAnimatorCollisionResponse* collisionLevel = manager->createCollisionResponseAnimator(
			levelSelector, robot->node, core::vector3df(7, 7, 10),
			core::vector3df(0, -100, 0), core::vector3df(0, 0, 1));

	// We add the animator to our collisionNode and drop the selector and collision if
	// we don't need it anymore.
	robot->node->addAnimator(collisionLevel);
	levelSelector->drop();
	collisionLevel->drop();

	// Set a jump of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	camera = manager->addCameraSceneNodeFPS(0, 100.0f, .3f, -1, 0, 0, true, 3.f);

	// Create list to keep track of enemies
	enemies = core::list<Enemy*>();

	// Tell the scene about this list of actors, so that removed actors also gets removed from this list
	addActorList((core::list<EffActor*>*) &enemies);

	// Creating an enemy and give it the parameters from the Enemy.cpp class
	enemies.push_back(new Enemy(manager, core::vector3df(-30, 0, -55), robot->node, .05));
	enemies.push_back(new Enemy(manager, core::vector3df(30, 0, -55), robot->node, .03));

	for(core::list<Enemy*>::Iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		// Add the actor to the scene
		this->addActor ((EffActor*) (*enemy));

		// Add collision with the player and the enemies
		(*enemy)->addCollision((scene::ISceneNode*) robot->node, ((scene::IMeshSceneNode*) robot->mesh->node)->getMesh());
	}

	// Set mouse Visible to false
	setMouseVisible(false);
	
	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	// Set the camera position and rotation plus
	// the camera follows the robot.
	camera->setPosition(vector3df(0, 40, 80));
	camera->setRotation(vector3df(0, 180, 0));
	robot->node->addChild(camera);

	return true;
}

void MainScene::update(float deltaTime)
{
	EffScene::update(deltaTime);

	// Set where the camera has to look at
	camera->setTarget(robot->node->getPosition());
	
	// When the spacebar is pressed and the cooldown is low enough, shoot!
	if (getInput()->IsKeyDown(irr::KEY_SPACE))
	{
		robot->shoot(&enemies);
	}

	// Check if there was collision with an enemy
	core::vector3df collisionPosition;
	for(core::list<Enemy*>::Iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		// If there was collision tell the enemy that it can hit the robot
		if(!(*enemy)->isDeath && (*enemy)->collisionOccurred(&collisionPosition)) {
			(*enemy)->hit (robot, collisionPosition);
			break;
		}
	}
}

MainScene::~MainScene(void)
{
	delete robot;
}
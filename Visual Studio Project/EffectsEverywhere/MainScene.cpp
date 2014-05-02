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
#include "EffTimer.h"
#include "TemporaryParticleEffect.h"

MainScene::MainScene()
{
	
}

bool MainScene::init(void)
{
	EffScene::init ();

	// Showcase particle to show that particle spawning by xml is working
	//scene::IParticleSystemSceneNode* node = this->pManager->spawnXMLParticle("../../Media/test.xml", core::vector3df(0,0,0));
	
	// Hit effect on the robot it is passed to the robot and then to the bullet
	ParticleModel* pModel = new ParticleModel();
	pModel->setEmitterType(ParticleModel::EmitterTypes::POINT);
	pModel->setMinColor(video::SColor(0,180,180,180));
	pModel->setMaxColor(video::SColor(0, 255, 255, 255));
	pModel->setMinPPS(50);
	pModel->setMaxPPS(200);
	pModel->setAabbox(core::aabbox3df(-3, 0, -3, 3, 1, 3 ));
	pModel->setDirection(core::vector3df(0.02f, 0.0f, 0.0f));
	pModel->setLifeTimeMax(400);
	pModel->setLifeTimeMin(350);
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
	robot->setWeapon("../../Media/rock-gun.obj","../../Media/rock-bullet.obj", 2, 0.6, 600, pModel, pModel, pModel);
	
	// Add floor to scene
	scene::IMesh* floorMesh = manager->getMesh("../../Media/level.obj");
	scene::IMeshSceneNode* floor = manager->addMeshSceneNode(floorMesh);
	if (!floor) return false;
	floor->setMaterialFlag(EMF_LIGHTING, false);

	// Random particles in the level
	pManager->spawnXMLParticle("../../Media/levelParticles.xml", core::vector3df(4,2,2));

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

	// Set mouse Visible to false
	setMouseVisible(false);
	
	// Add the camera node to the scene
	camera = manager->addCameraSceneNode();

	// Set the camera position and rotation plus
	// the camera follows the robot.
	camera->setPosition(vector3df(0, 40, 80));
	camera->setRotation(vector3df(0, 180, 0));
	robot->node->addChild(camera);

	spawnEnemy ();
	timer->repeat(std::bind(&MainScene::spawnEnemy, this), 2);

	return true;
}

void MainScene::spawnEnemy (void)
{
	std::cout << "Spawn enemy" << std::endl;

	// Create enemy
	Enemy* enemy = new Enemy(manager, core::vector3df(120, 0, -115), robot->node, .05 + (0.03 * (rand() / (float) RAND_MAX)));

	// Add to enemy list
	enemies.push_back(enemy);

	// Add the actor to the scene
	this->addActor ((EffActor*) enemy);

	// Add collision with the player and the enemies
	enemy->addCollision((scene::ISceneNode*) robot->node, ((scene::IMeshSceneNode*) robot->mesh->node)->getMesh());
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
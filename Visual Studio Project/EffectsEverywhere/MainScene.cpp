#include <iostream>
#include "MainScene.h"
#include "GameEngine.h"
#include "GameScene.h"
#include "InputReceiver.h"
#include "Enemy.h"

MainScene::MainScene(GameEngine* engine)
{
	_engine = engine;

	// Set the whole bullet array at NULL so that we can check if the item is NULL or not later on
	for (int i = 0; i < 10; i++) {
		bullets[i] = NULL;
	}

	// Active bullet index starts at zero
	bulletIndex = 0;
}

void MainScene::start(void)
{
	// This boolean is used to make sure that the emitter will be disabled after the particle cooldown
	hasEmitter = true;

	/** We set the particleScene node to a nullpointer so it can be first created before we disable the emitter
	 * otherwise it will break and lose memory
	 **/
	particleSceneNode = nullptr;

	//  boolean used for player and enemy collision
	particleOnCooldown = false;

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

	// create a hilleplanemesh to simulate height so we can create waves for the water particle
	 IMesh* watermesh = _engine->smgr->addHillPlaneMesh("watermesh",dimension2d<f32>(20, 20),dimension2d<u32>(2.5f,2.5f),0,0,dimension2d<f32>(0,0),dimension2d<f32>(10,10));
	 ISceneNode* waternode = _engine->smgr->addWaterSurfaceSceneNode(_engine->smgr->getMesh("watermesh"),2.0f,300.0f,30.0f);

	 if(waternode){
		 waternode->setPosition(vector3df(-60, 5, 60));
		 waternode->setMaterialTexture(0, _engine->driver->getTexture("../../Media/water.jpg"));
		 waternode->setMaterialTexture(1, _engine->driver->getTexture("../../Media/water.jpg"));
		 waternode->setMaterialType(EMT_REFLECTION_2_LAYER);
		 waternode->setMaterialFlag(EMF_LIGHTING,false);
	 }

	//Set a jump of 3 units per second, which gives a fairly realistic jump
	// when used with the gravity of (0, -10, 0) in the collision response animator.
	camera = _engine->smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, -1, 0, 0, true, 3.f);

	// Creating an enemy and give it the parameters from the Enemy.cpp class
	enemy1 = new Enemy(_engine, core::vector3df(-30, 0, -55), .05);
	enemy2 = new Enemy(_engine, core::vector3df(30, 0, -55), .03);

	// Add collision with the player and the enemies
	enemy1->addCollision(robot);
	enemy2->addCollision(robot);

	// Set mouse Visible to false
	_engine->setMouseVisible(false);
	
	// Add the camera node to the scene
	camera = _engine->smgr->addCameraSceneNode();

	// Set the camera position and rotation plus
	// the camera follows the robot.
	camera->setPosition(vector3df(0, 40, 80));
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
		// Add the player as the target of the enemies
		enemy1->setTarget(robot);
		enemy2->setTarget(robot);

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

	enemy1->update(_engine->deltaTime);
	enemy2->update(_engine->deltaTime);

	// Check if there was collision with enemy 1 and log the position if so
	core::vector3df collisionPosition;
	/*if (enemy1->collisionOccurred(&collisionPosition)) {
		std::cout << "Player got hit at " << collisionPosition.X << ", " << collisionPosition.Y << ", " << collisionPosition.Z << std::endl;
	}*/

	if(particleCooldown <= 0)
	{
		if(enemy1->collisionOccurred(&collisionPosition) && !particleOnCooldown){
			spawnParticleEffect(robot->getPosition());
			particleOnCooldown = true;
			particleCooldown = 100;
			std::cout << "enemy 1" << std::endl;
		}
		else if(enemy2->collisionOccurred(&collisionPosition) && !particleOnCooldown){
			spawnParticleEffect(robot->getPosition());
			particleOnCooldown = true;
			particleCooldown = 100;
			std::cout << "enemy 2" << std::endl;
		}else
			particleOnCooldown = false;
	}

	// Reduce the cooldown of shooting
	if (shootCooldown > 0) {
		shootCooldown -= _engine->deltaTime;
	}

	/**
	 * This if function checks if the particle cooldown != null and the cooldown is lower than 0 
	 * if so, it will disable the emitter but this will not remove/drop it. This needs to be done when the 
	 * enemy dies.
	 * WARNING the particleCooldown must always be lower than the shootCooldown otherwise it will never come into this
	 * if function and the particle will be constantly emitting. TIP: if u want to make a particle that needs to be there all
	 * the time then just set the particleCooldown equal to or higher than the shootCooldown
	 **/
	if (particleCooldown <= 0 && particleSceneNode != nullptr) {
		particleSceneNode->setEmitter(0);
	}
	// Reduce the cooldown of the particle
	if (particleCooldown > 0) {
		particleCooldown -= _engine->deltaTime;
	}
	
	// When the spacebar is pressed and the cooldown is low engouh, shoot!
	if (_engine->inputReceiver->IsKeyDown(irr::KEY_SPACE) && shootCooldown <= 0)
	{
		// Calculate the start and end of the ray and pass the intersection variable to get the collision position
		core::vector3df intersection;
		core::vector3df forward = core::vector3df(mat[2], 0, mat[0] * -1);

		// Set the beginning of the ray just a bit forward so that it doesnt hit the robot mesh
		if (checkRayCastIntersection(robot->getPosition() + (forward * 5), robot->getPosition() + (forward * 1000.), intersection)&& !particleOnCooldown)
		{
			/** Spawn a particle at the place of the collision
			 * the particle is created in the gamescene
			 * Set the cooldown of the particle
			 **/
			spawnParticleEffect (intersection);
			particleCooldown = 250;
			particleOnCooldown = true;
		}
		// Reset the cooldown
		shootCooldown = 350;

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
void MainScene::spawnParticleEffect (core::vector3df position) 
{
	//creating a particlesystemscenenode which basicly is a particle
	particleSceneNode = _engine->smgr->addParticleSystemSceneNode(false);
	
	//creating an emitter so u actually emit the particle from somewhere so it will be visual( in this case it's a box )
	Emitter = particleSceneNode->createBoxEmitter(aabbox3df(-3, 0, -3, 3, 1, 3 ),vector3df(0.0f, 0.1f, 0.0f),50,200,SColor(0, 0, 0, 255),
		SColor(0,255,255,255),500,750,0,dimension2df(4.0f, 4.0f), dimension2df(8.0f, 8.0f));

	// adding a particle affector which causes the particles to fade out
	IParticleAffector* paf = particleSceneNode->createFadeOutParticleAffector();

    particleSceneNode->addAffector(paf); // same goes for the affector
    paf->drop();

	//add the emitter to the particle and drop to prevent memory leakage
	particleSceneNode->setEmitter(Emitter);

	//check if the particlesystemscenenode is created correctly
	if(particleSceneNode){
		particleSceneNode->setPosition(position);
		particleSceneNode->setScale(vector3df(0.5f, 0.5f,0.5f));
		particleSceneNode->setMaterialFlag(EMF_LIGHTING, false);
		particleSceneNode->setMaterialFlag(EMF_ZWRITE_ENABLE, false);
		particleSceneNode->setMaterialTexture(0, _engine->driver->getTexture("../../Media/fireball.bmp"));
		particleSceneNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	}
}

MainScene::~MainScene(void)
{
	this->Emitter->drop();
}

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

	// Create robot actor
	robot = new Robot ();
	addNodeActor ((EffActor*) robot, core::vector3df(0, 7.5f, 0), core::vector3df(0, 0, 0));
	if (!robot) return false;

	// add Gun & Bullet
	robot->setWeapon("../../Media/rock-gun.obj","../../Media/rock-bullet.obj", 2, 0.6, 600, "../../Media/shootParticle.xml",130 , "../../Media/HitEffectE.xml",400 , "../../Media/RockTrailEffect.xml",200 );

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

	createHUD();

	spawnEnemy ();
	timer->repeat(std::bind(&MainScene::spawnEnemy, this), 2);

	return true;
}

void MainScene::spawnEnemy (void)
{
	std::cout << "Spawn enemy" << std::endl;

	// Create enemy
	Enemy* enemy = new Enemy(std::bind(&MainScene::onEnemyDies, this), manager, core::vector3df(120, 0, -115), robot->node, .05 + (0.03 * (rand() / (float) RAND_MAX)));

	// Create spawn particle effect
	TemporaryParticleEffect* p = new TemporaryParticleEffect(800);
	this->addXMLParticleActor((EffActor*) p, "../../Media/purpleEnemySpawnEffect.xml", core::vector3df(120, 0, -113));

	// Add to enemy list
	enemies.push_back(enemy);

	// Add the actor to the scene
	this->addActor ((EffActor*) enemy);

	// Add collision with the player and the enemies
	enemy->addCollision((scene::ISceneNode*) robot->node, ((scene::IMeshSceneNode*) robot->mesh->node)->getMesh());
}

void MainScene::createHUD(void)
{
	// Create a custom font
	IGUISkin* skin = gui->getSkin();
	IGUIFont* font = gui->getFont("../../media/fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);

	skin->setFont(gui->getBuiltInFont(), EGDF_TOOLTIP);

	// Add images of the hearts to the GUI
	gui->addImage(this->getTexture("../../Media/irrlichtlogo2.png"),
		core::position2d<int>(this->getDriverWidth() - 800, this->getDriverHeight() - 600));

	scorePlus = 0;
	score = L"Score: ";
	core::stringw tempScore = score;
	tempScore += scorePlus;

	// Add score and xp text to the GUI
	scoreText = gui->addStaticText(L"Score: ", rect<s32>(350,20,425,40), false);
	scoreText->setOverrideColor(video::SColor(255,255,100,255));
	scoreText->setText(tempScore.c_str());

	xpPlus = 0;
	xp = L"Xp: ";
	core::stringw tempXp = xp;
	tempXp += xpPlus;

	xpText = gui->addStaticText(L"Xp: ", rect<s32>(650,20,725,40), false);
	xpText->setOverrideColor(video::SColor(255,255,100,255));
	xpText->setText(tempScore.c_str());
}

void MainScene::onEnemyDies(void)
{
	scorePlus++;
	core::stringw yolo = score;
	yolo += scorePlus;
	scoreText->setText(yolo.c_str());

	xpPlus++;
	core::stringw swag = xp;
	swag += xpPlus;
	xpText->setText(swag.c_str());
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

	if (getInput()->IsKeyDown(irr::KEY_KEY_1))
	{
		robot->setWeapon("../../Media/rock-gun.obj","../../Media/rock-bullet.obj", 2, 0.6, 600, "../../Media/shootParticle.xml",200, "../../Media/HitEffectE.xml",400, "../../Media/RockTrailEffect.xml",200);
	}
	if (getInput()->IsKeyDown(irr::KEY_KEY_2))
	{
		robot->setWeapon("../../Media/rock-gun.obj","../../Media/rock-bullet.obj", 5, 0.6, 800, "../../Media/ToxicShootEffect.xml",800, "../../Media/ToxicHitEffect.xml",250, "../../Media/ToxicTrailEffect.xml",200);
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
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
#include <string>
#include "StartScene.h"
#include "WaveSystem.h"
#include "Wave.h"
#include "SpawnPoint.h"

MainScene::MainScene()
{
	spawnPoint1 = core::vector2df(120, -115);
	spawnPoint2 = core::vector2df(110, 110);
	spawnPoint3 = core::vector2df(-100, 115);
}

bool MainScene::init(void)
{
	EffScene::init ();
	TemporaryParticleEffect* p = new TemporaryParticleEffect(1400);
	this->addXMLParticleActor((EffActor*) p, "../../Media/SpawnP1.xml", core::vector3df(0, 100,0));
	// Create robot actor
	robot = new Robot (std::bind(&MainScene::onPlayerHit, this));
	addNodeActor ((EffActor*) robot, core::vector3df(0, 127.5f, 0), core::vector3df(0, 0, 0));
	if (!robot) return false;

	// add Gun & Bullet
	robot->setWeapon("../../Media/rock-gun.obj", // gun mesh
			core::vector3df(-8.5, 7, 0), // gun position
			"../../Media/rock-bullet.obj", // bullet mesh
			core::vector3df(0, 0, 0), // bullet offset
			2, // damage
			0.6, // speed
			600, // cooldown
			"../../Media/shootParticle.xml", // shoot effect
			200, // shoot effect lifetime
			"../../Media/HitEffectE.xml", // hit effect
			400, // hit effect lifetime
			"../../Media/RockTrailEffect.xml", // bullet trail effect
			200); // bullet trail effect life time

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
	collisionLevel = manager->createCollisionResponseAnimator(
			levelSelector, robot->node, core::vector3df(7, 7, 10),
			core::vector3df(0, -0.1, 0), core::vector3df(0, 0, 1));

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
	levelstart = true;
	createHUD();
	return true;
}

void MainScene::startPlaying(void)
{
	robot->node->addChild(camera);
	waveSystem = new WaveSystem();
	AddWaves();
	waveSystem->start();
	collisionLevel->setGravity(core::vector3df(0, -100, 0));
}

void MainScene::spawnEnemy (core::vector2df position, Enemy::TYPES type)
{
	std::cout << "Spawn enemy" << std::endl;

	// Create enemy
	Enemy* enemy = new Enemy(
		std::bind(&MainScene::onEnemyDie, this),
		manager,
		type,
		core::vector3df(position.X, 0, position.Y),
		robot->node,
		.05 + (0.03 * (rand() / (float) RAND_MAX))
	);

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
	IGUIFont* font = gui->getFont("../../Media/fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);

	skin->setFont(gui->getBuiltInFont(), EGDF_TOOLTIP);

	gui->addImage(this->getTexture("../../Media/hud-bar.png"),
		core::position2d<int>(this->getDriverWidth()-800, this->getDriverHeight()-600));

	//Score
	score = 0;
	scoreText = gui->addStaticText(L"Score: 0", rect<s32>(370,12,445,40), false);
	scoreText->setOverrideColor(video::SColor(255,31,31,31));
	scoreText->setText((core::stringw("Score: ") + core::stringw(score)).c_str());

	// Xp
	xp = 0;
	xpText = gui->addStaticText(L"Xp: 0", rect<s32>(725,12,800,40), false);
	xpText->setOverrideColor(video::SColor(255,31,31,31));
	xpText->setText((core::stringw("Xp: ") + core::stringw(xp)).c_str());

	//Health
	healthText = gui->addStaticText(L"Health: 100" , rect<s32>(30,12,105,40), false);
	healthText->setOverrideColor(video::SColor(255,31,31,31));
	healthText->setText((core::stringw("Health: ") + core::stringw(robot->health)).c_str());
}

void MainScene::onEnemyDie(void)
{
	score++;
	scoreText->setText((core::stringw("Score: ") + core::stringw(score)).c_str());

	xp++;
	xpText->setText((core::stringw("Xp: ") + core::stringw(xp)).c_str());
}

void MainScene::onPlayerHit(void)
{
	healthText->setText((core::stringw("Health: ") + core::stringw(robot->health)).c_str());
}

void MainScene::update(float deltaTime)
{
	EffScene::update(deltaTime);

	
	if(robot->node->getPosition().Y < 8 && levelstart && robot->node->getRotation().Y < -368)
	{
		levelstart = false;
		this->startPlaying();
	}
	if (levelstart)
	{
		core::vector3df rot=robot->node->getRotation();
		robot->node->setRotation(core::vector3df(rot.X,rot.Y += -.3,rot.Z));
	}
	// Set where the camera has to look at
	camera->setTarget(robot->node->getPosition());
	
	// When the spacebar is pressed and the cooldown is low enough, shoot!
	if (getInput()->IsMLeftButtonDown() || getInput()->IsKeyDown(irr::KEY_SPACE))
	{
		robot->shoot(&enemies);
	}
	//Switch weapon
	if (getInput()->IsKeyDown(irr::KEY_KEY_1))
	{
		robot->setWeapon("../../Media/rock-gun.obj", // gun mesh
			core::vector3df(-8.5, 7, 0), // gun position
			"../../Media/rock-bullet.obj", // bullet mesh
			core::vector3df(0, 0, 0), // bullet offset
			2, // damage
			0.6, // speed
			600, // cooldown
			"../../Media/shootParticle.xml", // shoot effect
			200, // shoot effect lifetime
			"../../Media/HitEffectE.xml", // hit effect
			400, // hit effect lifetime
			"../../Media/RockTrailEffect.xml", // bullet trail effect
			200); // bullet trail effect life time
	}
	if (getInput()->IsKeyDown(irr::KEY_KEY_2))
	{
		robot->setWeapon("../../Media/toxic-gun.obj",
			core::vector3df(-1, 2, 0), // gun position
			"../../Media/toxic-bullet.obj",
			core::vector3df(-7, 0, 0), // bullet offset
			5,
			1,
			800,
			"../../Media/ToxicShootEffect.xml",
			800,
			"../../Media/ToxicHitEffect.xml",
			250,
			"../../Media/ToxicTrailEffect.xml",
			200);
	}

	if (getInput()->IsKeyDown(irr::KEY_ESCAPE))
	{
		switchScene(new StartScene());
		return;
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

void MainScene::AddWaves (void)
{
	// Wave 1
	core::list<int> wave1p1enemies = core::list<int>();
	wave1p1enemies.push_back(1);
	wave1p1enemies.push_back(0);
	core::list<int> wave1p2enemies = core::list<int>();
	wave1p2enemies.push_back(2);
	wave1p2enemies.push_back(0);
	core::list<int> wave1p3enemies = core::list<int>();
	wave1p3enemies.push_back(1);
	wave1p3enemies.push_back(0);

	core::list<SpawnPoint> wave1points = core::list<SpawnPoint>();
	wave1points.push_back(SpawnPoint(spawnPoint1, wave1p1enemies));
	wave1points.push_back(SpawnPoint(spawnPoint2, wave1p2enemies));
	wave1points.push_back(SpawnPoint(spawnPoint3, wave1p3enemies));
	
	waveSystem->addWave(Wave(this, wave1points, 5));

	// Wave 2
	core::list<int> wave2p1enemies = core::list<int>();
	wave2p1enemies.push_back(1);
	wave2p1enemies.push_back(1);
	core::list<int> wave2p2enemies = core::list<int>();
	wave2p2enemies.push_back(2);
	wave2p2enemies.push_back(2);
	core::list<int> wave2p3enemies = core::list<int>();
	wave2p3enemies.push_back(1);
	wave2p3enemies.push_back(1);

	core::list<SpawnPoint> wave2points = core::list<SpawnPoint>();
	wave2points.push_back(SpawnPoint(spawnPoint1, wave2p1enemies));
	wave2points.push_back(SpawnPoint(spawnPoint2, wave2p2enemies));
	wave2points.push_back(SpawnPoint(spawnPoint3, wave2p3enemies));

	waveSystem->addWave(Wave(this, wave2points, 3));
}

MainScene::~MainScene(void)
{
	delete robot;
}
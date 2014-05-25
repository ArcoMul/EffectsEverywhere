#include <iostream>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include "MainScene.h"
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "Enemy.h"
#include "PurpleEnemy.h"
#include "EvilEnemy.h"
#include "Bullet.h"
#include "Robot.h"
#include "EffTimer.h"
#include "TemporaryParticleEffect.h"
#include <string>
#include "StartScene.h"
#include "WaveSystem.h"
#include "Wave.h"
#include "SpawnPoint.h"
#include "GuiAnimation.h"
#include "EndScene.h"

MainScene::MainScene()
{
	spawnPoint1 = core::vector2df(120, -115);
	spawnPoint2 = core::vector2df(110, 110);
	spawnPoint3 = core::vector2df(-100, 115);

	enemiesAlive = 0;
}

bool MainScene::init(void)
{
	EffScene::init ();
	
	// Create robot actor
	robot = new Robot (std::bind(&MainScene::onPlayerHit, this));
	addNodeActor ((EffActor*) robot, core::vector3df(0, 127.5f, 0), core::vector3df(0, 0, 0));
	if (!robot) return false;

	TemporaryParticleEffect* p = new TemporaryParticleEffect(1900);
	this->addXMLParticleActor((EffActor*) p, "../../Media/SpawnP1.xml", core::vector3df(0, 133,0));

	TemporaryParticleEffect* p2 = new TemporaryParticleEffect(3300);
	this->addXMLParticleActor((EffActor*) p2, "../../Media/SpawnP2.xml", core::vector3df(0, 0,0));

	robot->node->addChild(p2->node);
	// add Gun & Bullet
	robot->setWeapon("../../Media/rock-gun.obj", // gun mesh
			core::vector3df(-8.5, 7, 0), // gun position
			"../../Media/rock-bullet.obj", // bullet mesh
			core::vector3df(0, 0, 0), // bullet offset
			5, // damage
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

	//lantern
	pManager->spawnXMLParticle("../../Media/lanternParticle.xml", core::vector3df(98,35,91.5f));
	//torch 1 barracks location
	pManager->spawnXMLParticle("../../Media/torchParticle.xml", core::vector3df(122,20,-86.5f));
	//torch 2 near spawn location
	pManager->spawnXMLParticle("../../Media/torchParticle.xml", core::vector3df(-106,20,-87.5f));

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
	levelWon = false;
	hudActive = false;
	createHUD();
	return true;
}

void MainScene::startPlaying(void)
{	
	robot->node->addChild(camera);
	camera->setPosition(camera->getPosition()-robot->node->getPosition());
	robot->setLevelStart(true);
	waveSystem = new WaveSystem(this);
	AddWaves();
	waveSystem->start();
	collisionLevel->setGravity(core::vector3df(0, -100, 0));
}

void MainScene::spawnEnemy (core::vector2df position, Enemy::TYPES type)
{
	enemiesAlive++;

	// Create enemy
	Enemy* enemy;
	if (type == Enemy::TYPES::EVIL) {
		enemy = (Enemy*) new EvilEnemy(std::bind(&MainScene::onEnemyDie, this), manager, core::vector3df(position.X, 0, position.Y), robot->node);
	} else {
		enemy = (Enemy*) new PurpleEnemy(std::bind(&MainScene::onEnemyDie, this), manager, core::vector3df(position.X, 0, position.Y), robot->node);
	}

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
	IGUIFont* font = gui->getFont("../../Media/fonts/asap.xml");
	if (font) {
		skin->setFont(font);
	}

	gui::IGUIImage* hudBar = gui->addImage(this->getTexture("../../Media/hud-bar.png"), core::position2d<int>(0, -55));
	hudBar->setScaleImage(true);
	hudBar->setMinSize(core::dimension2du(this->getDriverWidth(), 55));
	hudBarAnim = new GuiAnimation(hudBar, 0, 0, .5, 100);

	//Health
	healthText = gui->addStaticText(L"Health: 100" , rect<s32>(30, -40, 230, 0), false);
	healthText->setOverrideColor(video::SColor(255,31,31,31));
	healthText->setText((core::stringw("Health: ") + core::stringw(robot->health)).c_str());
	healthAnim = new GuiAnimation(healthText, 30, 10, .5, 350);

	//Score
	score = 0;
	scoreText = gui->addStaticText(L"Score: 0", rect<s32>((this->getDriverWidth() / 2) - 100, -40, (this->getDriverWidth() / 2) + 100, 50), false);
	scoreText->setOverrideColor(video::SColor(255,31,31,31));
	scoreText->setTextAlignment(gui::EGUI_ALIGNMENT::EGUIA_CENTER, gui::EGUI_ALIGNMENT::EGUIA_UPPERLEFT);
	scoreText->setText((core::stringw("Score: ") + core::stringw(score)).c_str());
	scoreAnim = new GuiAnimation(scoreText, (this->getDriverWidth() / 2) - 100, 10, .5, 350);

	// Xp
	xp = 0;
	xpText = gui->addStaticText(L"Xp: 0", rect<s32>(this->getDriverWidth() - 230, -40, this->getDriverWidth() - 30, 50), false);
	xpText->setOverrideColor(video::SColor(255,31,31,31));
	xpText->setTextAlignment(gui::EGUI_ALIGNMENT::EGUIA_LOWERRIGHT, gui::EGUI_ALIGNMENT::EGUIA_UPPERLEFT);
	xpText->setText((core::stringw("Xp: ") + core::stringw(xp)).c_str());
	xpAnim = new GuiAnimation(xpText, this->getDriverWidth() - 230, 10, .5, 350);

	// Wave text
	waveText = gui->addStaticText(L"", rect<s32>((this->getDriverWidth() / 2) - 100, 100, (this->getDriverWidth() / 2) + 100, 200), false);
	waveText->setOverrideColor(video::SColor(255,255,255,255));
	waveText->setTextAlignment(gui::EGUI_ALIGNMENT::EGUIA_CENTER, gui::EGUI_ALIGNMENT::EGUIA_UPPERLEFT);
}

void MainScene::onEnemyDie(void)
{
	enemiesAlive--;

	score++;
	scoreText->setText((core::stringw("Score: ") + core::stringw(score)).c_str());

	xp++;
	xpText->setText((core::stringw("Xp: ") + core::stringw(xp)).c_str());
	
	waveSystem->checkNextWave();
}

void MainScene::onPlayerHit(void)
{
	healthText->setText((core::stringw("Health: ") + core::stringw(robot->health)).c_str());
}

void MainScene::update(float deltaTime)
{
	EffScene::update(deltaTime);
	if (!levelstart &&(hudActive || (!robot->node->getPosition().X == 0 && !robot->node->getPosition().Z == 0))){
		hudActive = true;
		hudBarAnim->update(deltaTime);
		scoreAnim->update(deltaTime);
		xpAnim->update(deltaTime);
		healthAnim->update(deltaTime);
	}

	if(robot->node->getPosition().Y < 7.6 && levelstart && robot->node->getRotation().Y < -360)
	{
		levelstart = false;
		this->startPlaying();
	}
	if (levelstart && robot->node->getRotation().Y > -360)
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
			5, // damage
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
			10,
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
		switchScene(new EndScene(levelWon,score));
		return;
	}
	if (levelWon)
	{
		switchScene(new EndScene(levelWon,score));
		return;
	}
	// Check if there was collision with an enemy
	core::vector3df collisionPosition;
	for(core::list<Enemy*>::Iterator enemy = enemies.begin(); enemy != enemies.end(); enemy++)
	{
		// If there was collision tell the enemy that it can hit the robot
		if(!(*enemy)->isDeath && (*enemy)->collisionOccurred(&collisionPosition)) {
			(*enemy)->hit (robot, collisionPosition);
			// switch scene when the player dies
			if (robot->health <= 0) {
				switchScene(new EndScene(levelWon,score));
				return;
				}
			break;
		}
	}
}

void MainScene::showWaveText(core::stringw text)
{
	waveText->setText(text.c_str());
	waveText->setOverrideColor(video::SColor(255,255,255,255));
	timer->time(std::bind(&MainScene::hideWaveText, this), 2);
}

void MainScene::hideWaveText()
{
	waveText->setOverrideColor(video::SColor(0,255,255,255));
}

void MainScene::AddWaves (void)
{
	// Wave 1 - intro, only one cave
	core::list<int> wave1p2enemies = core::list<int>();
	wave1p2enemies.push_back(3);

	core::list<SpawnPoint> wave1points = core::list<SpawnPoint>();
	wave1points.push_back(SpawnPoint(spawnPoint2, wave1p2enemies));
	
	waveSystem->addWave(Wave(this, wave1points, 2));

	// Wave 2 - intro, more caves
	core::list<int> wave2p1enemies = core::list<int>();
	wave2p1enemies.push_back(3);
	core::list<int> wave2p3enemies = core::list<int>();
	wave2p3enemies.push_back(3);

	core::list<SpawnPoint> wave2points = core::list<SpawnPoint>();
	wave2points.push_back(SpawnPoint(spawnPoint1, wave2p1enemies));
	wave2points.push_back(SpawnPoint(spawnPoint3, wave2p3enemies));
	
	waveSystem->addWave(Wave(this, wave2points, 3));

	// Wave 3, - more enemies
	core::list<int> wave3p1enemies = core::list<int>();
	wave3p1enemies.push_back(2);
	core::list<int> wave3p2enemies = core::list<int>();
	wave3p2enemies.push_back(2);
	core::list<int> wave3p3enemies = core::list<int>();
	wave3p3enemies.push_back(2);

	core::list<SpawnPoint> wave3points = core::list<SpawnPoint>();
	wave3points.push_back(SpawnPoint(spawnPoint1, wave3p1enemies));
	wave3points.push_back(SpawnPoint(spawnPoint2, wave3p2enemies));
	wave3points.push_back(SpawnPoint(spawnPoint3, wave3p3enemies));
	
	waveSystem->addWave(Wave(this, wave3points, 4));
	
	// Wave 4, more types of enemies
	core::list<int> wave4p1enemies = core::list<int>();
	wave4p1enemies.push_back(1);
	wave4p1enemies.push_back(1);
	core::list<int> wave4p2enemies = core::list<int>();
	wave4p2enemies.push_back(1);
	core::list<int> wave4p3enemies = core::list<int>();
	wave4p3enemies.push_back(1);
	wave4p3enemies.push_back(1);

	core::list<SpawnPoint> wave4points = core::list<SpawnPoint>();
	wave4points.push_back(SpawnPoint(spawnPoint1, wave4p1enemies));
	wave4points.push_back(SpawnPoint(spawnPoint2, wave4p2enemies));
	wave4points.push_back(SpawnPoint(spawnPoint3, wave4p3enemies));
	
	waveSystem->addWave(Wave(this, wave4points, 3));

	// Wave 5, a lot of enemies in a short time
	core::list<int> wave5p2enemies = core::list<int>();
	wave5p2enemies.push_back(7);

	core::list<SpawnPoint> wave5points = core::list<SpawnPoint>();
	wave5points.push_back(SpawnPoint(spawnPoint2, wave5p2enemies));
	
	waveSystem->addWave(Wave(this, wave5points, 1));

	// Wave 6, a lot of enemies in a short time with different types
	core::list<int> wave6p1enemies = core::list<int>();
	wave6p1enemies.push_back(1);
	core::list<int> wave6p2enemies = core::list<int>();
	wave6p2enemies.push_back(4);
	wave6p2enemies.push_back(4);
	core::list<int> wave6p3enemies = core::list<int>();
	wave6p3enemies.push_back(1);

	core::list<SpawnPoint> wave6points = core::list<SpawnPoint>();
	wave6points.push_back(SpawnPoint(spawnPoint1, wave6p1enemies));
	wave6points.push_back(SpawnPoint(spawnPoint2, wave6p2enemies));
	wave6points.push_back(SpawnPoint(spawnPoint3, wave6p3enemies));
	
	waveSystem->addWave(Wave(this, wave6points, 1.5));

	// Wave 7, a lot of enemies everywhere!
	core::list<int> wave7p1enemies = core::list<int>();
	wave7p1enemies.push_back(2);
	wave7p1enemies.push_back(2);
	core::list<int> wave7p2enemies = core::list<int>();
	wave7p2enemies.push_back(4);
	wave7p2enemies.push_back(4);
	core::list<int> wave7p3enemies = core::list<int>();
	wave7p3enemies.push_back(2);
	wave7p3enemies.push_back(2);

	core::list<SpawnPoint> wave7points = core::list<SpawnPoint>();
	wave7points.push_back(SpawnPoint(spawnPoint1, wave7p1enemies));
	wave7points.push_back(SpawnPoint(spawnPoint2, wave7p2enemies));
	wave7points.push_back(SpawnPoint(spawnPoint3, wave7p3enemies));
	
	waveSystem->addWave(Wave(this, wave7points, 2.5));

	// Wave 8, a lot of evil enemies
	core::list<int> wave8p1enemies = core::list<int>();
	wave8p1enemies.push_back(0);
	wave8p1enemies.push_back(3);
	core::list<int> wave8p2enemies = core::list<int>();
	wave8p2enemies.push_back(0);
	wave8p2enemies.push_back(4);
	core::list<int> wave8p3enemies = core::list<int>();
	wave8p3enemies.push_back(0);
	wave8p3enemies.push_back(3);

	core::list<SpawnPoint> wave8points = core::list<SpawnPoint>();
	wave8points.push_back(SpawnPoint(spawnPoint1, wave8p1enemies));
	wave8points.push_back(SpawnPoint(spawnPoint2, wave8p2enemies));
	wave8points.push_back(SpawnPoint(spawnPoint3, wave8p3enemies));
	
	waveSystem->addWave(Wave(this, wave8points, 2.5));

	// Wave 9, too much
	core::list<int> wave9p1enemies = core::list<int>();
	wave9p1enemies.push_back(4);
	wave9p1enemies.push_back(4);
	core::list<int> wave9p2enemies = core::list<int>();
	wave9p2enemies.push_back(4);
	wave9p2enemies.push_back(4);
	core::list<int> wave9p3enemies = core::list<int>();
	wave9p3enemies.push_back(4);
	wave9p3enemies.push_back(4);

	core::list<SpawnPoint> wave9points = core::list<SpawnPoint>();
	wave9points.push_back(SpawnPoint(spawnPoint1, wave9p1enemies));
	wave9points.push_back(SpawnPoint(spawnPoint2, wave9p2enemies));
	wave9points.push_back(SpawnPoint(spawnPoint3, wave9p3enemies));
	
	waveSystem->addWave(Wave(this, wave9points, 3));
}

void MainScene::setLevelWon(bool levelWon){
	this->levelWon = levelWon;
}

MainScene::~MainScene(void)
{
	delete robot;
}
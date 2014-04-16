#include "Robot.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "Enemy.h"
#include "Bullet.h"
#include "TemporaryParticleEffect.h"
#include "Gun.h"
#include <iostream>
#include <ParticleManager.h>
#include <ParticleModel.h>

float damp (float acc, float damp)
{
	acc += acc > 0 ? -damp : damp;
	if (abs(acc) <= damp) {
		acc = 0;
	}
	return acc;
}

float limit (float value, float max)
{
	value = value > max ? max : value;
	value = value < -max ? -max : value;
	return value;
}

Robot::Robot(void)
{
	// Set some default values
	floatDirection = 1;
	restFloatSpeed = 0.005;
	movingFloatSpeed = 0.0075;
	floatSpeed = restFloatSpeed;
	bulletMesh ="null";
	maxAcceleration = .1;
	damping = .0005;
	health = 10;
}

void Robot::start ()
{
	EffActor::start();
	
	// Create the robot mesh
	mesh = new EffActor();
	scene->addMeshActor ((EffActor*) mesh, "../../Media/robot.obj", core::vector3df(0, 0, 0), core::vector3df(0, 0, 0));
	mesh->node->setMaterialFlag(video::EMF_LIGHTING, false);
	mesh->node->setParent(node);

	// Effect of fire beneath robot
	// TODO: base rotation of effect on rotation of robot
	ParticleModel* floatEffect = new ParticleModel();
	floatEffect->setEmitterType(ParticleModel::EmitterTypes::BOX);
	floatEffect->setMinColor(video::SColor(255, 69, 183, 255));
	floatEffect->setMaxColor(video::SColor(255, 69, 183, 255));
	floatEffect->setMinPPS(150);
	floatEffect->setMaxPPS(200);
	floatEffect->setAabbox(core::aabbox3df(-1.5, 0, -1.5, 1.5, 1.5, 1.5 ));
	floatEffect->setDirection(core::vector3df(0.0f, -0.05f, 0.0f));
	floatEffect->setLifeTimeMax(25);
	floatEffect->setLifeTimeMin(10);
	floatEffect->setMaxAngleDegrees(0);
	floatEffect->setMinStartSize(core::dimension2df(4.0f, 4.0f));
	floatEffect->setMaxStartSize(core::dimension2df(4.0f, 4.0f));
	floatEffect->setPathNameTexture("../../Media/floatingRobot.png");
	floatEffect->setPosition(core::vector3df(0,-4,0));
	EffActor* a = scene->addParticleActor(new EffActor(), floatEffect, floatEffect->getPosition());
	a->node->setParent(mesh->node);

	// Second effect when the enemy gets hit by a bullet
	// This is the Triangle Effect
	enemyTriangleHitEffectModel = new ParticleModel();
	enemyTriangleHitEffectModel->setEmitterType(ParticleModel::EmitterTypes::POINT);
	enemyTriangleHitEffectModel->setMinColor(video::SColor(0,0,0,0));
	enemyTriangleHitEffectModel->setMaxColor(video::SColor(0, 0, 0, 0));
	enemyTriangleHitEffectModel->setMinPPS(20);
	enemyTriangleHitEffectModel->setMaxPPS(80);
	enemyTriangleHitEffectModel->setAabbox(core::aabbox3df(-3, 0, -3, 3, 1, 3 ));
	enemyTriangleHitEffectModel->setDirection(core::vector3df(0.01f, 0.0f, 0.02f));
	enemyTriangleHitEffectModel->setLifeTimeMax(750);
	enemyTriangleHitEffectModel->setLifeTimeMin(550);
	enemyTriangleHitEffectModel->setMaxAngleDegrees(360);
	enemyTriangleHitEffectModel->setMinStartSize(core::dimension2df(4.0f, 4.0f));
	enemyTriangleHitEffectModel->setMaxStartSize(core::dimension2df(8.0f, 8.0f));
	enemyTriangleHitEffectModel->setPathNameTexture("../../Media/triangleEffect.png");
	enemyTriangleHitEffectModel->setPosition(core::vector3df(2,2,2));
}

void Robot::update(float deltaTime)
{
	EffActor::update(deltaTime);

	// Rotation and position of the robot
	core::vector3df rot = node->getRotation();
	core::vector3df pos = node->getPosition();

	// Rotation of the mesh
	core::vector3df meshRotation = mesh->node->getRotation();

	// Get the transformations done on this robot
	core::matrix4 mat = node->getAbsoluteTransformation();
 
	// When the W key is down
	if(scene->getInput()->IsKeyDown(irr::KEY_KEY_W)) {
		velocity.Z = -.0006;
	}
	// When the S key is down go back
	else if(scene->getInput()->IsKeyDown(irr::KEY_KEY_S)) {
		velocity.Z = .0006;
	} else {
		velocity.Z = 0;
	}
	
	// When the A key is down go right
	if(scene->getInput()->IsKeyDown(irr::KEY_KEY_A)) {
		velocity.X = .0006;
	}
	// When the D key is down go left
	else if(scene->getInput()->IsKeyDown(irr::KEY_KEY_D)) {
		velocity.X = -.0006;
	} else {
		velocity.X = 0;
	}

	// Change acceleration based on velocity
	acceleration.X += velocity.X;
	acceleration.Z += velocity.Z;
	if ( velocity.X == 0 && acceleration.X != 0) {
		acceleration.X = damp (acceleration.X, damping);
	}
	if ( velocity.Z == 0 && acceleration.Z != 0) {
		acceleration.Z = damp (acceleration.Z, damping);
	}

	// Movement speed
 	bool goingLeftAndRight = false;
 	// WARNING HACKY: When both a front/back key and a right/left key is pressed reduce the speed,
 	// so that it doesn't move twice as fast when going in a diagonal line
 	if ((scene->getInput()->IsKeyDown(irr::KEY_KEY_W) || scene->getInput()->IsKeyDown(irr::KEY_KEY_S))
 	&& (scene->getInput()->IsKeyDown(irr::KEY_KEY_A) || scene->getInput()->IsKeyDown(irr::KEY_KEY_D)))
 	goingLeftAndRight = true;

	// Limit the speed on the X and Z axis if the player is moving on both of these axis
	acceleration.X = limit (acceleration.X, (goingLeftAndRight ? maxAcceleration * 0.667 : maxAcceleration));
	acceleration.Z = limit (acceleration.Z, (goingLeftAndRight ? maxAcceleration * 0.667 : maxAcceleration));

	// Change the position based on the acceleration
	pos += core::vector3df(mat[0] * acceleration.X * deltaTime,
			0,
			mat[2] * acceleration.X * deltaTime);
	pos += core::vector3df(mat[2] * -acceleration.Z * deltaTime,
			0,
			mat[0] * acceleration.Z * deltaTime);

	// Change the rotation based on the acceleration
	meshRotation.Z = -10 * acceleration.X / maxAcceleration;
	meshRotation.X = 10 * acceleration.Z / maxAcceleration;

	// When moving apply different speed for floating
	if(scene->getInput()->IsKeyDown(irr::KEY_KEY_W) || scene->getInput()->IsKeyDown(irr::KEY_KEY_S) || scene->getInput()->IsKeyDown(irr::KEY_KEY_A) || scene->getInput()->IsKeyDown(irr::KEY_KEY_D)) {
		floatSpeed = movingFloatSpeed;
	} else {
		floatSpeed = restFloatSpeed;
	}

	// Get position of robot mesh
	core::vector3df meshPosition = mesh->node->getPosition();
	// Move it up or down based on floatSpeed
	meshPosition.Y += deltaTime * floatSpeed * floatDirection;
	// Reverse the float speed at the max-points
	if ((meshPosition.Y >= 4 && floatDirection > 0) || (meshPosition.Y <= 0 && floatDirection < 0)) floatDirection *= -1;
	// Apply
	mesh->node->setPosition (meshPosition);

	// Add deltaMouse, the change of mouse position, to the rotation of the robot
    rot.Y += -.3 * scene->getDeltaMouse().X;

	// Set the newly calculated position and rotation
	node->setPosition(pos);
	node->setRotation(rot);
	mesh->node->setRotation(meshRotation);

	// Reduce the cooldown of shooting
	if (countShootCooldown > 0) {
		countShootCooldown -= deltaTime;
	}
}

void Robot::setWeapon (core::stringc gunMesh, core::stringc bulletMesh, int damage, float speed, float cooldown, core::stringc shootEffect, core::stringc enemyHitEffect, core::stringc flyEffect)
{
	//Set gun/edit gun
	if(this->bulletMesh == "null"){
		addGun(gunMesh);
	}else
	{
		scene->removeActor((EffActor*) gun);
		addGun(gunMesh);
	}
	
	// Set default cooldown
	this->shootCooldown = cooldown;

	// Set default bulletMesh
	this->bulletMesh = bulletMesh;
	
	// Set default damage
	this->bulletDamage = damage;
	
	// Set default speed
	this->bulletSpeed = speed;

	// Set the effects
	this->shootEffectXML = shootEffect;
	this->enemyHitEffectXML = enemyHitEffect;
	this->flyEffectXML = flyEffect;
}

void Robot::setWeapon (core::stringc gunMesh, core::stringc bulletMesh, int damage, float speed, float cooldown,ParticleModel* shootEffect, ParticleModel* enemyHitEffect, ParticleModel* flyEffect)
{
	//Set gun/edit gun
	if(this->bulletMesh == "null"){
		addGun(gunMesh);
	}else
	{
		scene->removeActor((EffActor*) gun);
		addGun(gunMesh);
	}

	// Set default cooldown
	this->shootCooldown = cooldown;

	// Set default bulletMesh
	this->bulletMesh = bulletMesh;
	
	// Set default damage
	this->bulletDamage = damage;
	
	// Set default speed
	this->bulletSpeed = speed;

	// Set the effects
	this->shootEffectModel = shootEffect;
	this->enemyHitEffectModel = enemyHitEffect;
	this->flyEffectModel = flyEffect;
}

void Robot::addGun(core::stringc gunMesh)
{
	// Create gun actor
	gun = new Gun();
	scene->addMeshActor ((EffActor*) gun, gunMesh);
	gun->node->setParent (mesh->node);

	// Put the gun on the right position
	core::matrix4 mat = node->getAbsoluteTransformation();
	core::vector3df right = core::vector3df(-mat[0], 0, -mat[2]);
	gun->node->setPosition(node->getPosition() + (right * 8.5) - core::vector3df(0, 4, 0));
}

void Robot::shoot (core::list<Enemy*>* enemies)
{
	if (bulletMesh == "null") return;
	if (countShootCooldown > 0) return;
	
	core::matrix4 mat = node->getAbsoluteTransformation();

	// Calculate the start and end of the ray and pass the intersection variable to get the collision position
	core::vector3df intersection;
	core::vector3df forward = core::vector3df(mat[2], 0, -mat[0]);

	// Reset the cooldown
	countShootCooldown = shootCooldown;

	// Create bullet actor with the right position and rotation
	Bullet* bullet = new Bullet(enemies, bulletSpeed, bulletDamage, enemyHitEffectModel, enemyTriangleHitEffectModel);
	scene->addMeshActor ((EffActor*) bullet, bulletMesh, gun->node->getAbsolutePosition(), node->getRotation());

	gun->shoot();

	// Create a shoot effect
	shootParticleModel = new ParticleModel();
	shootParticleModel->setEmitterType(ParticleModel::EmitterTypes::POINT);
	shootParticleModel->setMinColor(video::SColor(0,180,180,180));
	shootParticleModel->setMaxColor(video::SColor(0, 255, 255, 255));
	shootParticleModel->setMinPPS(50);
	shootParticleModel->setMaxPPS(200);
	shootParticleModel->setDirection(core::vector3df(0.05f, 0.0f, 0.0f));
	shootParticleModel->setLifeTimeMax(750);
	shootParticleModel->setLifeTimeMin(500);
	shootParticleModel->setMaxAngleDegrees(360);
	shootParticleModel->setMinStartSize(core::dimension2df(3.0f, 3.0f));
	shootParticleModel->setMaxStartSize(core::dimension2df(6.0f, 6.0f));
	shootParticleModel->setPathNameTexture("../../Media/smoke.png");
	TemporaryParticleEffect* shootEffect = new TemporaryParticleEffect(130, false);
	scene->addParticleActor((EffActor*) shootEffect, shootParticleModel, gun->node->getPosition() + core::vector3df(0,0,-7));
	shootEffect->node->setParent(mesh->node);

	IParticleSystemSceneNode* particleNode = (IParticleSystemSceneNode*) shootEffect->node;
	scene::IParticleAffector* affector = particleNode->createFadeOutParticleAffector();
	particleNode->addAffector(affector);
	affector->drop();
}

void Robot::hit (int damage, core::vector3df position)
{
	health -= damage;

	ParticleModel* hitEffect = new ParticleModel();
	hitEffect->setEmitterType(ParticleModel::EmitterTypes::BOX);
	hitEffect->setMinColor(video::SColor(0, 255, 255, 255));
	hitEffect->setMaxColor(video::SColor(0, 255, 255, 255));
	hitEffect->setMinPPS(50);
	hitEffect->setMaxPPS(200);
	hitEffect->setAabbox(core::aabbox3df(-3, 0, -3, 3, 1, 3 ));
	hitEffect->setDirection(core::vector3df(0.0f, 0.1f, 0.0f));
	hitEffect->setLifeTimeMax(750);
	hitEffect->setLifeTimeMin(500);
	hitEffect->setMaxAngleDegrees(0);
	hitEffect->setMinStartSize(core::dimension2df(4.0f, 4.0f));
	hitEffect->setMaxStartSize(core::dimension2df(8.0f, 8.0f));
	hitEffect->setPathNameTexture("../../Media/portal1.bmp");
	TemporaryParticleEffect* p = new TemporaryParticleEffect(500);
	scene->addParticleActor((EffActor*) p, hitEffect, position);
	IParticleSystemSceneNode* particleNode = (IParticleSystemSceneNode*) p->node;
	scene::IParticleAffector* affector = particleNode->createFadeOutParticleAffector();
	particleNode->addAffector(affector);
	affector->drop();

	// Remove the player and stop the scene when the player dies
	// (not a beautiful way, but is serves the purpose)
	if (health <= 0) {
		scene->stop();
		mesh->node->remove();
		scene->removeActor((EffActor*) this);
	}
}

Robot::~Robot(void)
{
}

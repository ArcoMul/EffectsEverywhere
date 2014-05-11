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
	EffActor* a = scene->addXMLParticleActor(new EffActor(),"../../Media/floatEffect.xml", core::vector3df(0,-4,0));
	a->node->setParent(mesh->node);
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
	Bullet* bullet = new Bullet(enemies, bulletSpeed, bulletDamage, enemyHitEffectXML, flyEffectXML);
	scene->addMeshActor ((EffActor*) bullet, bulletMesh, gun->node->getAbsolutePosition(), node->getRotation());

	gun->shoot();

	TemporaryParticleEffect* shootEffect = new TemporaryParticleEffect(130, false);
	scene->addXMLParticleActor((EffActor*) shootEffect,shootEffectXML.c_str(), gun->node->getPosition() + core::vector3df(0,0,-7));
	shootEffect->node->setParent(mesh->node);

	IParticleSystemSceneNode* particleNode = (IParticleSystemSceneNode*) shootEffect->node;
	scene::IParticleAffector* affector = particleNode->createFadeOutParticleAffector();
	particleNode->addAffector(affector);
	affector->drop();
}

void Robot::hit (int damage, core::vector3df position)
{
	health -= damage;
	
	TemporaryParticleEffect* p = new TemporaryParticleEffect(500);
	scene->addXMLParticleActor((EffActor*) p, "../../Media/HitEffectR.xml", position);

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

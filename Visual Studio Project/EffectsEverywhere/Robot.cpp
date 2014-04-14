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

Robot::Robot(void)
{
	// Set some default values
	floatDirection = 1;
	restFloatSpeed = 0.005;
	movingFloatSpeed = 0.0075;
	floatSpeed = restFloatSpeed;
	defbulletMesh ="null";
	// TODO: Fill in with right information for the shoot effect when point emitter is supported
	// this is not used now
	shootParticleModel = new ParticleModel();
	shootParticleModel->setEmitterType(ParticleModel::EmitterTypes::BOX);
	shootParticleModel->setMinColor(video::SColor(0,0,0,255));
	shootParticleModel->setMaxColor(video::SColor(0, 255, 255, 255));
	shootParticleModel->setMinPPS(50);
	shootParticleModel->setMaxPPS(200);
	shootParticleModel->setAabbox(core::aabbox3df(-3, 0, -3, 3, 1, 3 ));
	shootParticleModel->setDirection(core::vector3df(0.0f, 0.0f, 0.0f));
	shootParticleModel->setLifeTimeMax(750);
	shootParticleModel->setLifeTimeMin(500);
	shootParticleModel->setMaxAngleDegrees(0);
	shootParticleModel->setMinStartSize(core::dimension2df(4.0f, 4.0f));
	shootParticleModel->setMaxStartSize(core::dimension2df(8.0f, 8.0f));
	shootParticleModel->setPathNameTexture("../../Media/fireball.bmp");
}

void Robot::start ()
{
	EffActor::start();
	
	// Create the robot mesh
	mesh = new EffActor();
	scene->addMeshActor ((EffActor*) mesh, "../../Media/robot.obj", core::vector3df(0, 0, 0), core::vector3df(0, 0, 0));
	mesh->node->setMaterialFlag(video::EMF_LIGHTING, false);
	mesh->node->setParent(node);

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

	// Movement speed
 	float speed = .1;

 	// WARNING HACKY: When both a front/back key and a right/left key is pressed reduce the speed,
 	// so that it doesn't move twice as fast when going in a diagonal line
 	if ((scene->getInput()->IsKeyDown(irr::KEY_KEY_W) || scene->getInput()->IsKeyDown(irr::KEY_KEY_S))
 	&& (scene->getInput()->IsKeyDown(irr::KEY_KEY_A) || scene->getInput()->IsKeyDown(irr::KEY_KEY_D)))
 	speed *= 0.667; // 0.667 is sort of the factor of the distance you move when you go in a 45 degree angle
 
	// When the W key is down
	if(scene->getInput()->IsKeyDown(irr::KEY_KEY_W))
	{
		// Multiply the already done transformations of the robot with the speed and deltaTime
		pos += core::vector3df(mat[2] * speed * deltaTime,
		 	0,
			mat[0] * -speed * deltaTime);
		meshRotation.X = -10;
	}
	// When the S key is down go back
	else if(scene->getInput()->IsKeyDown(irr::KEY_KEY_S))
	{
		pos += core::vector3df(mat[2] * -speed * deltaTime,
			0,
			mat[0] * speed * deltaTime);
		meshRotation.X = 10;
	}
	else {
		meshRotation.X = 0;
	}
	
	// When the A key is down go right
	if(scene->getInput()->IsKeyDown(irr::KEY_KEY_A))
	{
		pos += core::vector3df(mat[0] * speed * deltaTime,
			0,
			mat[2] * speed * deltaTime);
		meshRotation.Z = -10;
	}
	// When the D key is down go left
	else if(scene->getInput()->IsKeyDown(irr::KEY_KEY_D))
	{
		pos += core::vector3df(mat[0] * -speed * deltaTime,
			0,
			mat[2] * -speed * deltaTime);
		meshRotation.Z = 10;
	}
	else {
		meshRotation.Z = 0;
	}

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
	if (meshPosition.Y > 4 || meshPosition.Y < 0) floatDirection *= -1;
	// Apply
	mesh->node->setPosition (meshPosition);

	// Add deltaMouse, the change of mouse position, to the rotation of the robot
    rot.Y += -.3 * scene->getDeltaMouse().X;

	// Set the newly calculated position and rotation
	node->setPosition(pos);
	node->setRotation(rot);
	mesh->node->setRotation(meshRotation);

	// Reduce the cooldown of shooting
	if (shootCooldown > 0) {
		shootCooldown -= deltaTime;
	}
}

void Robot::weapon (core::stringc gunMesh, core::stringc bulletMesh, int damage, float speed, float cooldown, core::stringc shootEffect, core::stringc enemyHitEffect, core::stringc flyEffect)
{
	// Create gun actor
	gun = new Gun();
	scene->addMeshActor ((EffActor*) gun, gunMesh);
	gun->node->setParent (mesh->node);

	// Put the gun on the right position
	core::matrix4 mat = node->getAbsoluteTransformation();
	core::vector3df right = core::vector3df(-mat[0], 0, -mat[2]);
	gun->node->setPosition(node->getPosition() + (right * 8.5) - core::vector3df(0, 4, 0));
	
	// Set default cooldown
	defshootCooldown = cooldown;

	// Set default bulletMesh
	defbulletMesh = bulletMesh;
	
	// Set default damage
	defbulletDamage = damage;
	
	// Set default speed
	defbulletSpeed = speed;

	// Set the effects
	shootEffectXML = shootEffect;
	enemyHitEffectXML = enemyHitEffect;
	flyEffectXML = flyEffect;
}

void Robot::weapon (core::stringc gunMesh, core::stringc bulletMesh, int damage, float speed, float cooldown,ParticleModel* shootEffect, ParticleModel* enemyHitEffect, ParticleModel* flyEffect)
{
	// Create gun actor
	gun = new Gun();
	scene->addMeshActor ((EffActor*) gun, gunMesh);
	gun->node->setParent (mesh->node);

	// Put the gun on the right position
	core::matrix4 mat = node->getAbsoluteTransformation();
	core::vector3df right = core::vector3df(-mat[0], 0, -mat[2]);
	gun->node->setPosition(node->getPosition() + (right * 8.5) - core::vector3df(0, 4, 0));
	
	// Set default cooldown
	defshootCooldown = cooldown;

	// Set default bulletMesh
	defbulletMesh = bulletMesh;
	
	// Set default damage
	defbulletDamage = damage;
	
	// Set default speed
	defbulletSpeed = speed;

	// Set the effects
	shootEffectModel = shootEffect;
	enemyHitEffectModel = enemyHitEffect;
	flyEffectModel = flyEffect;
}

void Robot::shoot (core::list<Enemy*>* enemies)
{
	if (defbulletMesh == "null") return;
	if (shootCooldown > 0) return;
	
	core::matrix4 mat = node->getAbsoluteTransformation();

	// Calculate the start and end of the ray and pass the intersection variable to get the collision position
	core::vector3df intersection;
	core::vector3df forward = core::vector3df(mat[2], 0, -mat[0]);

	// Reset the cooldown
	shootCooldown = defshootCooldown;

	// Create bullet actor with the right position and rotation
	Bullet* bullet = new Bullet(enemies, defbulletSpeed, defbulletDamage, enemyHitEffectModel);
	scene->addMeshActor ((EffActor*) bullet, defbulletMesh, gun->node->getAbsolutePosition(), node->getRotation());

	gun->shoot();

	// Create a shoot effect
	TemporaryParticleEffect* shootEffect = new TemporaryParticleEffect(130, false);
	scene->addPointParticleActor ((EffActor*) shootEffect, gun->node->getPosition() + core::vector3df(0,0,-7));

	// Set some specific settings
	// TODO: convert to particle model
	scene::IParticleSystemSceneNode* particleNode = (scene::IParticleSystemSceneNode*) shootEffect->node;
	particleNode->setScale(core::vector3df(2, 2, 2));
	particleNode->setMaterialTexture(0, scene->getTexture("../../Media/smoke.png"));
	particleNode->setMaterialFlag(video::EMF_LIGHTING, false);
	particleNode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	particleNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	particleNode->setParent (mesh->node);
}

void Robot::hit (core::vector3df position)
{
	// Spawn a particle effect where the robot was hit
	TemporaryParticleEffect* p = new TemporaryParticleEffect(500);
	scene->addParticleActor ((EffActor*) p, position);

	// Set some specific settings
	// TODO: convert to particle model
	scene::IParticleSystemSceneNode* particleNode = (scene::IParticleSystemSceneNode*) p->node;
	particleNode->setScale(core::vector3df(0.5f, 0.5f,0.5f));
	particleNode->setMaterialTexture(0, scene->getTexture("../../Media/portal1.bmp"));
	particleNode->setMaterialFlag(video::EMF_LIGHTING, false);
	particleNode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	particleNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
}

Robot::~Robot(void)
{
}

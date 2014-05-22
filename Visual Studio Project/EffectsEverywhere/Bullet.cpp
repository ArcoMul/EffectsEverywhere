#include "EffScene.h"
#include "Bullet.h"
#include "Enemy.h"
#include <iostream>
#include <ParticleManager.h>
#include <ParticleModel.h>

Bullet::Bullet (core::list<Enemy*>* enemies, float bulletSpeed, int damage, core::stringc enemyHitEffectXML, core::stringc flyEffectXML)
{
	this->enemies = enemies;
	this->speed = bulletSpeed;
	this->lifeTime = 1000;
	this->damage = damage;
	this->enemyHitEffectXML = enemyHitEffectXML;
	this->flyEffectXML = flyEffectXML;
}

void Bullet::start ()
{
	EffActor::start();

	// Scene is available here
	aliveSince = scene->getTime();

	// The node is set now, set some properties
	node->setMaterialFlag(video::EMF_LIGHTING, false);

	trailEffect = new EffActor();
	scene->addXMLParticleActor((EffActor*) trailEffect, flyEffectXML.c_str(), core::vector3df(0,0,0));
	trailEffect->node->setParent(node);
}

void Bullet::update (float deltaTime)
{
	EffActor::update(deltaTime);

	// Remove the bullet if it is alive for 500 miliseconds
	if (scene->getTime() - aliveSince > lifeTime)
	{
		scene->removeActor((EffActor*) this);
		return;
	}

	for(core::list<Enemy*>::Iterator enemy = enemies->begin(); enemy != enemies->end(); enemy++)
	{
		if((*enemy)->node != nullptr && node->getTransformedBoundingBox().intersectsWithBox((*enemy)->node->getTransformedBoundingBox()))
		{
			// Spawn a particle effect at the position where we hit something with the bullet
			EffActor* p = new EffActor();
			scene->addXMLParticleActor(p,enemyHitEffectXML.c_str(),node->getPosition());

			(*enemy)->hit(damage);
			scene->removeActor ((EffActor*) trailEffect);
			scene->removeActor ((EffActor*) this);
			return;
		}
	}

	// Get the current position and rotation and calculate based on that the new position,
	// mostly forward and a bit down
	core::vector3df pos = node->getPosition();
	core::matrix4 mat = node->getAbsoluteTransformation();
	pos += core::vector3df(mat[2] * speed * deltaTime,
		-.015 * deltaTime,
		mat[0] * -speed * deltaTime);
	node->setPosition(pos);

}

Bullet::~Bullet(void)
{
}

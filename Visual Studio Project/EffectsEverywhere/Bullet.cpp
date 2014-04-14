#include "EffScene.h"
#include "Bullet.h"
#include "Enemy.h"
#include "TemporaryParticleEffect.h"
#include <iostream>

Bullet::Bullet (core::list<Enemy*>* enemies, float bulletSpeed, int damage)
{
	this->enemies = enemies;
	this->speed = bulletSpeed;
	this->lifeTime = 1000;
	this->damage = damage;
}

void Bullet::start ()
{
	EffActor::start();

	// Scene is available here
	aliveSince = scene->getTime();

	// The node is set now, set some properties
	node->setMaterialFlag(video::EMF_LIGHTING, false);
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
			TemporaryParticleEffect* p = new TemporaryParticleEffect(node->getPosition(), 250, "../../Media/fireball.bmp");
			scene->addParticleActor ((EffActor*) p);

			(*enemy)->hit(damage);
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

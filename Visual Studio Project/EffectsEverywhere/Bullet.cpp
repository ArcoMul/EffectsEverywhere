#include "EffScene.h"
#include "Bullet.h"
#include <iostream>

Bullet::Bullet(void)
{
}

Bullet::Bullet (core::vector3df position, core::vector3df rotation)
{
	// Save the position, we can't set these yet since the node isn't
	// set yet probably
	this->startPosition = position;
	this->startRotation = rotation;
}

void Bullet::start ()
{
	EffActor::start();

	// Scene is available here
	aliveSince = scene->getTime();

	// The node is set now, set some properties
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setPosition(startPosition);
	node->setRotation(startRotation);
}

void Bullet::update (float deltaTime)
{
	EffActor::update(deltaTime);

	// Remove the bullet if it is alive for 500 miliseconds
	if (scene->getTime() - aliveSince > 500)
	{
		scene->removeActor((EffActor*) this);
		return;
	}

	// Get the current position and rotation and calculate based on that the new position
	core::vector3df pos = node->getPosition();
	core::matrix4 mat = node->getAbsoluteTransformation();
	pos += core::vector3df(mat[2] * .5 * deltaTime,
		0,
		mat[0] * -.5 * deltaTime);
	node->setPosition(pos);
}

Bullet::~Bullet(void)
{
	
}

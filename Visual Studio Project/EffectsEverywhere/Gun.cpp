#include "Gun.h"
#include <cmath>

Gun::Gun(void)
{
	direction = 1;
	shooting = false;
}

void Gun::start ()
{
	EffActor::start();

	node->setMaterialFlag(video::EMF_LIGHTING, false);
}

void Gun::update(float deltaTime)
{
	EffActor::update(deltaTime);

	// When the gun is not 'shooting' nothing happens here
	if (!shooting) return;

	// Get the current position and make the gun move in the right direction
	core::vector3df pos = node->getPosition();
	pos += core::vector3df(0, 0, .1 * direction);
	
	// When the gun moved enough, move it back, change the direction
	if (pos.Z > 3)
	{
		direction *= -1;
	}

	// When the gun is back on its original place, stop shooting and make sure to reset for sure
	if (pos.Z < 0)
	{
		shooting = false;
		pos.Z = 0;
	}

	// Apply the new position
	node->setPosition(pos);
}

void Gun::shoot ()
{
	shooting = true;
	direction = 1;
}

Gun::~Gun(void)
{
}

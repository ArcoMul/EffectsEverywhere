#include "Bullet.h"


Bullet::Bullet(void)
{
}

Bullet::Bullet(scene::IMeshSceneNode* node, float aliveSince)
{
	this->node = node;
	this->aliveSince = aliveSince;
}


Bullet::~Bullet(void)
{
}

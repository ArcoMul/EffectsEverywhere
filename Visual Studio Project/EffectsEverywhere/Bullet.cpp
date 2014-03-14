#include "Bullet.h"

Bullet::Bullet(void)
{
}

Bullet::Bullet(float aliveSince)
{
	this->aliveSince = aliveSince;
}

void Bullet::update (float deltaTime)
{
	EffActor::update(deltaTime);
}

Bullet::~Bullet(void)
{
}

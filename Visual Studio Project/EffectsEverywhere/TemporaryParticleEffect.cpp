#include "TemporaryParticleEffect.h"
#include "EffScene.h"

TemporaryParticleEffect::TemporaryParticleEffect (float lifeTime, bool fade)
{
	// Save the properties for later use
	this->lifeTime = lifeTime;
	this->fade = fade;
}

void TemporaryParticleEffect::start(void)
{
	// Save when the particle is spawned
	aliveSince = scene->getTime();
}

void TemporaryParticleEffect::update(float deltaTime)
{
	EffActor::update(deltaTime);

	// When the lifetime is over start the removal of this effect
	if (scene->getTime() - aliveSince > lifeTime)
	{
		// First set the emittor to zero, there will no new particles be spawned
		((scene::IParticleSystemSceneNode*) node)->setEmitter(0);

		// Give the particles 2 extra seconds to dissapear after deleting the whole irrlicht node
		if (!fade || scene->getTime() - aliveSince > lifeTime + 2000)
		{
			scene->removeActor((EffActor*) this);
		}
	}
}

TemporaryParticleEffect::~TemporaryParticleEffect(void)
{
}

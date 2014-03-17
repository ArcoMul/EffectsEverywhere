#include "TemporaryParticleEffect.h"
#include "EffScene.h"

TemporaryParticleEffect::TemporaryParticleEffect (core::vector3df position, float lifeTime, core::stringc texturePath)
{
	// Save the properties for later use
	this->spawnPosition = position;
	this->lifeTime = lifeTime;
	this->texturePath = texturePath;
}

void TemporaryParticleEffect::start(void)
{
	// The node is set, set extra properties
	scene::IParticleSystemSceneNode* particleNode = (scene::IParticleSystemSceneNode*) node;
	particleNode->setScale(core::vector3df(0.5f, 0.5f,0.5f));
	particleNode->setMaterialTexture(0, scene->getTexture(texturePath));
	particleNode->setMaterialFlag(video::EMF_LIGHTING, false);
	particleNode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	particleNode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	// Save when the particle is spawned
	aliveSince = scene->getTime();

	// Put the node in the right position
	node->setPosition(spawnPosition);
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
		if (scene->getTime() - aliveSince > lifeTime + 2000)
		{
			scene->removeActor((EffActor*) this);
		}
	}
}

TemporaryParticleEffect::~TemporaryParticleEffect(void)
{
}

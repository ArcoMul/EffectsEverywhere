#ifndef TEMPORARYPARTICLEEFFECT_H_
#define TEMPORARYPARTICLEEFFECT_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class EffScene;

/**
 * The temprory particle effect is a particle effect with some default settings which
 * gets deleted after a certain lifetime.
 */
class TemporaryParticleEffect : public EffActor
{
public:
	TemporaryParticleEffect (float lifeTime, bool fade = true);

	/**
	 * Set some properties to the particle effect bound to its node
	 */
	void virtual start (void);

	/**
	 * In the update call we check if it is time to remove the particle
	 */
	void virtual update (float deltaTime);

	~TemporaryParticleEffect(void);

private:
	
	/**
	 * The position to spawn the particle
	 */
	core::vector3df spawnPosition;

	/**
	 * Since when the particle is alive
	 */
	float aliveSince;

	/**
	 * How long the particle should life
	 */
	float lifeTime;

	/**
	 * Which texture the particle should have
	 */
	core::stringc texturePath;

	bool fade;
};

#endif
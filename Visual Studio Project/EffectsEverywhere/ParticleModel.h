#ifndef PARTICLEMODEL_H_
#define PARTICLEMODEL_H_

#include <irrlicht.h>

using namespace irr;

class ParticleModel
{
public:
	ParticleModel(void);
	void switchEmitterType();
	~ParticleModel(void);

	enum emitterType
	{
		BOX,
		POINT,
		ANIMATED_MESH,
		CYLINDER,
		MESH,
		RING,
		SPHERE,
		NONE = 0
	};

private:
	void createBoxEmittingParticle();
	void createPointEmittingParticle();
	void createAnimatedMeshEmittingParticle();
	void createCylinderEmittingParticle();
	void createMeshEmittingParticle();
	void createRingEmittingParticle();
	void createSphereEmittingParticle();
};

#endif
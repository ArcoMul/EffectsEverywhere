#ifndef PARTICLEMODEL_H_
#define PARTICLEMODEL_H_

#include <irrlicht.h>

using namespace irr;
using namespace video;

class ParticleModel
{
public:
	ParticleModel(void);
	void switchEmitterType();
	~ParticleModel(void);

	enum EmitterTypes
	{
		NONE = 0,
		BOX,
		POINT,
		ANIMATED_MESH,
		CYLINDER,
		MESH,
		RING,
		SPHERE
	};

	EmitterTypes emitterType;
	core::aabbox3df aabbox;
	core::vector3df direction;
	s32 maxAngleDegrees;
	u32 lifeTimeMax,lifeTimeMin, maxPPS, minPPS;
	core::stringc pathNameTexture;
	const video::SColor &minStartColor, &maxStartColor;
	const core::dimension2df &minStartSize, &maxStartSize;

private:

};

#endif
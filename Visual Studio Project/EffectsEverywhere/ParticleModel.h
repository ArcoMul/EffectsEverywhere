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

	enum EmitterType
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

	aabbox3df<f32> aabbox;
	vector3df direction;
	s32 maxAngleDegrees;
	u32 lifeTimeMax,lifeTimeMin, maxPPS, minPPS;
	ITexture* texture;
	const video::SColor &color;
	const core::dimension2df &size;

private:

};

#endif
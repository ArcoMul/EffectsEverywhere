#ifndef PARTICLEMANAGER_H_
#define PARTICLEMANAGER_H_

#include <irrlicht.h>
#include "ParticleModel.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;

class ParticleModel;

class ParticleManager
{
public:
	ParticleManager(video::IVideoDriver* driver, IrrlichtDevice* device, ISceneManager* smgr);
	IParticleSystemSceneNode* spawnDataModelParticle(ParticleModel* model, vector3df position);
	IParticleSystemSceneNode* spawnXMLParticle(stringc xmlname, vector3df position);
	~ParticleManager(void);

	ISceneManager* smgr;
	IVideoDriver* driver;
	IrrlichtDevice* device;

private:
	void createBoxEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode);
	void createPointEmittingParticle(ParticleModel* particleModel);
	void createAnimatedMeshEmittingParticle();
	void createCylinderEmittingParticle();
	void createMeshEmittingParticle();
	void createRingEmittingParticle();
	void createSphereEmittingParticle();
};

#endif
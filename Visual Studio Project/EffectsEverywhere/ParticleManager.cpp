#include <irrlicht.h>
#include "ParticleManager.h"

ParticleManager::ParticleManager(video::IVideoDriver* driver, IrrlichtDevice* device, ISceneManager* smgr)
{
	this->driver = driver;
	this->device = device;
	this->smgr = smgr;
}

IParticleSystemSceneNode* ParticleManager::spawnDataModelParticle(ParticleModel* model, vector3df position)
{
	IParticleSystemSceneNode* particleNode = smgr->addParticleSystemSceneNode(false);
	switch(model->emitterType)
	{
	case(model->BOX):
		createBoxEmittingParticle(model,particleNode);
		break;
	case(model->POINT):
		createPointEmittingParticle();
		break;
	case(model->ANIMATED_MESH):
		createAnimatedMeshEmittingParticle();
		break;
	case(model->CYLINDER):
		createCylinderEmittingParticle();
		break;
	case(model->MESH):
		createMeshEmittingParticle();
		break;
	case(model->RING):
		createRingEmittingParticle();
		break;
	case(model->SPHERE):
		createSphereEmittingParticle();
		break;
	case(model->NONE):
		break;
	}

	return particleNode;
}

IParticleSystemSceneNode* ParticleManager::spawnXMLParticle(stringc xmlname, vector3df position)
{
	//TODO: when the parser is there this is the function that needs to be used instead of the dataModel
}

void ParticleManager::createBoxEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode)
{
	IParticleBoxEmitter* boxEmitter = particleNode->createBoxEmitter(particleModel->aabbox,particleModel->direction,particleModel->minPPS,
		particleModel->maxPPS,particleModel->minStartColor,particleModel->maxStartColor,particleModel->lifeTimeMin,particleModel->lifeTimeMax,particleModel->maxAngleDegrees,
		particleModel->minStartSize,particleModel->maxStartSize);
	particleNode->setEmitter(boxEmitter);
	boxEmitter->drop();
}

void ParticleManager::createPointEmittingParticle(ParticleModel* particleModel)
{

}

ParticleManager::~ParticleManager(void)
{
}
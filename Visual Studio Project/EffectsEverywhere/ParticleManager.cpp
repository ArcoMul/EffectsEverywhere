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
		createPointEmittingParticle(model,particleNode);
		break;
	case(model->ANIMATED_MESH):
		createAnimatedMeshEmittingParticle(model,particleNode);
		break;
	case(model->CYLINDER):
		createCylinderEmittingParticle(model,particleNode);
		break;
	case(model->MESH):
		createMeshEmittingParticle(model,particleNode);
		break;
	case(model->RING):
		createRingEmittingParticle(model,particleNode);
		break;
	case(model->SPHERE):
		createSphereEmittingParticle(model,particleNode);
		break;
	case(model->NONE):
		break;
	}

	return particleNode;
}

IParticleSystemSceneNode* ParticleManager::spawnXMLParticle(stringc xmlname, vector3df position)
{
	IParticleSystemSceneNode* particleNode = smgr->addParticleSystemSceneNode(false);
	//TODO: when the parser is there this is the function that needs to be used instead of the dataModel
	return particleNode;
}

void ParticleManager::createBoxEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode)
{
	IParticleBoxEmitter* boxEmitter = particleNode->createBoxEmitter(particleModel->aabbox,particleModel->direction,particleModel->minPPS,
		particleModel->maxPPS,particleModel->minStartColor,particleModel->maxStartColor,particleModel->lifeTimeMin,particleModel->lifeTimeMax,particleModel->maxAngleDegrees,
		particleModel->minStartSize,particleModel->maxStartSize);
	particleNode->setEmitter(boxEmitter);
	boxEmitter->drop();
}

void ParticleManager::createPointEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode)
{
	/*IParticlePointEmitter* pointEmitter = particleNode->createPointEmitter();
	particleNode->setEmitter(pointEmitter);
	pointEmitter->drop();*/
}

void ParticleManager::createAnimatedMeshEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode)
{
	/*scene::IParticleAnimatedMeshSceneNodeEmitter* animatedMeshEmitter = particleNode->createAnimatedMeshSceneNodeEmitter();
	particleNode->setEmitter(animatedMeshEmitter);
	animatedMeshEmitter->drop();*/
}

void ParticleManager::createCylinderEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode)
{
	/*scene::IParticleCylinderEmitter* cylinderEmitter = particleNode->createCylinderEmitter();
	particleNode->setEmitter(cylinderEmitter);
	cylinderEmitter->drop();*/
}

void ParticleManager::createMeshEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode)
{

}

void ParticleManager::createRingEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode)
{

}

void ParticleManager::createSphereEmittingParticle(ParticleModel* particleModel,IParticleSystemSceneNode* particleNode)
{

}

ParticleManager::~ParticleManager(void)
{
}
#include "EffScene.h"
#include "Bullet.h"
#include "Enemy.h"
#include "TemporaryParticleEffect.h"
#include <iostream>
#include <ParticleManager.h>
#include <ParticleModel.h>

Bullet::Bullet (core::list<Enemy*>* enemies, float bulletSpeed, int damage, core::stringc enemyHitEffectXML,float enemyHitEffectLifeTime, core::stringc flyEffectXML, float flyEffectLifeTime)
{
	this->enemies = enemies;
	this->speed = bulletSpeed;
	this->lifeTime = 1000;
	this->damage = damage;
	this->enemyHitEffectXML = enemyHitEffectXML;
	this->flyEffectXML = flyEffectXML;
	this->enemyHitEffectLifeTime = enemyHitEffectLifeTime;
	this->flyEffectLifeTime = flyEffectLifeTime;
}

void Bullet::start ()
{
	EffActor::start();

	// Scene is available here
	aliveSince = scene->getTime();

	// The node is set now, set some properties
	node->setMaterialFlag(video::EMF_LIGHTING, false);

	trailEffect = new TemporaryParticleEffect(500, false);
	ParticleParser* p = new ParticleParser();
	ParticleModel m = p->parse(flyEffectXML.c_str());
	m.addAffectorType(ParticleModel::AffectorTypes::SCALE);
	m.addAffectorType(ParticleModel::AffectorTypes::FADE_OUT);
	m.addAffectorType(ParticleModel::AffectorTypes::GRAVITY);
	m.setGravityAffectorGravity(core::vector3df(0,-0.05,0));
	m.setGravityAffectorTimeForceLost(500);
	m.setScaleAffectorScaleTo(core::dimension2df(15,15));
	//m.setFadeOutAffectorTargetColor(video::SColor(0,125,125,125));
	m.setFadeOutAffectorTimeNeededToFadeOut(3000);
	scene->addParticleActor((EffActor*) trailEffect, &m, core::vector3df(0,0,0));
	trailEffect->node->setParent(node);
	/*trailEffect->node->setMaterialType(video::EMT_ONETEXTURE_BLEND);
	trailEffect->node->getMaterial(0).MaterialTypeParam = video::pack_textureBlendFunc (video::EBF_SRC_ALPHA, video::EBF_ONE_MINUS_SRC_ALPHA,
                                                    video::EMFN_MODULATE_1X, video::EAS_TEXTURE | video::EAS_VERTEX_COLOR);*/
	// Undo these '//' to make the rock trail effect be what it was. We need to make is possible to change the material flags.
}

void Bullet::update (float deltaTime)
{
	EffActor::update(deltaTime);

	// Remove the bullet if it is alive for 500 miliseconds
	if (scene->getTime() - aliveSince > lifeTime)
	{
		scene->removeActor((EffActor*) this);
		return;
	}

	for(core::list<Enemy*>::Iterator enemy = enemies->begin(); enemy != enemies->end(); enemy++)
	{
		if((*enemy)->node != nullptr && node->getTransformedBoundingBox().intersectsWithBox((*enemy)->node->getTransformedBoundingBox()))
		{
			// Spawn a particle effect at the position where we hit something with the bullet
			TemporaryParticleEffect* p = new TemporaryParticleEffect(enemyHitEffectLifeTime, false);
			scene->addXMLParticleActor(p,enemyHitEffectXML.c_str(),node->getPosition());

			// Spawn a second particle effect at the position where we hit something with the bullet
			/*TemporaryParticleEffect* pTriangle = new TemporaryParticleEffect(enemyTriangleHitEffectModel->getLifeTimeMax(), false);
			scene->addParticleActor(pTriangle,enemyTriangleHitEffectModel, node->getPosition());

			IParticleSystemSceneNode* triangleParticleNode = (IParticleSystemSceneNode*)
			pTriangle->node;
			triangleParticleNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			*/

			(*enemy)->hit(damage);
			scene->removeActor ((EffActor*) trailEffect);
			scene->removeActor ((EffActor*) this);
			return;
		}
	}

	// Get the current position and rotation and calculate based on that the new position,
	// mostly forward and a bit down
	core::vector3df pos = node->getPosition();
	core::matrix4 mat = node->getAbsoluteTransformation();
	pos += core::vector3df(mat[2] * speed * deltaTime,
		-.015 * deltaTime,
		mat[0] * -speed * deltaTime);
	node->setPosition(pos);

}

Bullet::~Bullet(void)
{
}

#ifndef BULLET_H_
#define BULLET_H_

#include <irrlicht.h>
#include <ParticleManager.h>
#include <ParticleModel.h>
#include "EffActor.h"

using namespace irr;

class Enemy;
class TemporaryParticleEffect;

class Bullet : public EffActor
{

public:

	/**
	 * Default constructor
	 */
	Bullet (core::list<Enemy*>* enemies, float bulletSpeed, int demage, core::stringc enemyHitEffectXML, float enemyHitEffectLifeTime, core::stringc flyEffectXML, float flyEffectLifeTime);

	/**
	 * When the actor is ready apply some material settings to the node
	 * and set the right position and rotation
	 */
	virtual void start ();

	/**
	 * EffActor update method, called each frame. Update position of 
	 * bullet and check if it should be removed
	 */
	virtual void update (float deltaTime);

	/**
	 * Deconstructor
	 */
	~Bullet (void);

private:

	/**
	 * The time this bullet was spawned
	 */
	float aliveSince;

	float speed;

	int lifeTime;

	int damage;

	core::list<Enemy*>* enemies;


	core::stringc enemyHitEffectXML;
	core::stringc flyEffectXML;
	float enemyHitEffectLifeTime;
	float flyEffectLifeTime;
	TemporaryParticleEffect* trailEffect;
};

#endif

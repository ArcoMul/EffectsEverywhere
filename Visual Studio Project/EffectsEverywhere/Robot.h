#ifndef ROBOT_H_
#define ROBOT_H_

#include <irrlicht.h>
#include "EffActor.h"
#include <ParticleManager.h>
#include <ParticleModel.h>

using namespace irr;

class EffEngine;
class Bullet;
class Enemy;
class Gun;
class ParticleModel;

class Robot : public EffActor
{
public:
	Robot (void);

	/**
	 * Create the robot mesh and the gun node and position these on the right position
	 */
	virtual void start (void);

	/**
	 * Move the robot, make it float, and reduce the cooldown
	 */
	virtual void update (float deltaTime);

	/**
	 * Add gun and bullet 
	 */
	void setWeapon (core::stringc gunMesh, core::stringc bulletMesh, int damage,
		float speed, float cooldown, core::stringc shootEffect,float shootEffectLifeTime,
		core::stringc enemyHitEffect,float enemyHitEffectLifeTime, core::stringc flyEffect, float flyEffectLifeTime);
	
	
	/**
	 * Shoot a bullet from the gun
	 */
	void shoot (core::list<Enemy*>* enemies);

	/**
	 * The robot gets hit on a certain position
	 */
	void hit (int damage, core::vector3df position);

	~Robot(void);

	/**
	 * The mesh of the robot, the robot itself
	 */
	EffActor* mesh;

private:

	/**
	 * Add gun
	 */
	void addGun(core::stringc gunMesh);

	/**
	 * URL XMLs of the weapon effects
	 */
	core::stringc shootEffectXML;
	core::stringc enemyHitEffectXML;
	core::stringc flyEffectXML;

	/**
	 * Lifetime of the weapon effects
	 */
	float shootEffectLifeTime;
	float enemyHitEffectLifeTime;
	float flyEffectLifeTime;

	/**
	 * Weapon Cooldown
	 */
	float shootCooldown;

	/**
	 * URL Bullet Mesh
	 */
	core::stringc bulletMesh;

	/**
	 * Bullet Damage
	 */
	int bulletDamage;

	/**
	 * Bullet speed
	 */
	float bulletSpeed;

	/**
	 * Counter to keep track of the shoot cooldown
	 */
	float countShootCooldown;

	/**
	 * Gun actor
	 */
	Gun* gun;

	/**
	 * Which direction is the robot floating, up or down, 1 or -1
	 */
	int floatDirection;

	/**
	 * When the robot doesn't move it floats more slowly
	 */
	float restFloatSpeed;

	/**
	 * When the robot moves the float speed is more fast
	 */
	float movingFloatSpeed;

	/**
	 * The current float speed
	 */
	float floatSpeed;

	core::vector3df acceleration;
	float maxAcceleration;
	float damping;
	core::vector3df velocity;

	int health;
};

#endif

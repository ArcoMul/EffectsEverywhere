#ifndef ROBOT_H_
#define ROBOT_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class EffEngine;
class Bullet;
class Enemy;
class Gun;

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
	 * Shoot a bullet from the gun
	 */
	void weapon (core::stringc gunMesh, core::stringc bulletMesh, int damage, float speed, float cooldown, core::stringc shootEffect, core::stringc enemyHitEffect, core::stringc flyRffect);

	/**
	 * Shoot a bullet from the gun
	 */
	void shoot (core::list<Enemy*>* enemies);

	/**
	 * The robot gets hit on a certain position
	 */
	void hit (core::vector3df position);

	~Robot(void);

	/**
	 * The mesh of the robot, the robot itself
	 */
	EffActor* mesh;

private:
	/**
	 * URL XMLs of the weapon effects
	 */
	core::stringc shootEffectXML;
	core::stringc enemyHitEffectXML;
	core::stringc flyRffectXML;

	/**
	 * Weapon Cooldown
	 */
	float defshootCooldown;

	/**
	 * URL Bullet Mesh
	 */
	core::stringc defbulletMesh;

	/**
	 * Bullet Damage
	 */
	int defbulletDamage;

	/**
	 * Bullet speed
	 */
	float defbulletSpeed;

	/**
	 * Counter to keep track of the shoot cooldown
	 */
	float shootCooldown;

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
};

#endif

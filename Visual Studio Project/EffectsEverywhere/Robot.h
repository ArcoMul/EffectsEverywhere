#ifndef ROBOT_H_
#define ROBOT_H_

#include <irrlicht.h>
#include "EffActor.h"
#include <functional>
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
	
	Robot (std::function<void(void)> onHit);

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
	void setWeapon (core::stringc gunMesh, core::vector3df gunPosition, core::stringc bulletMesh, core::vector3df bulletOffset, int damage,
		float speed, float cooldown, core::stringc shootEffect, core::stringc enemyHitEffect, core::stringc flyEffect);
	
	
	/**
	 * Shoot a bullet from the gun
	 */
	void shoot (core::list<Enemy*>* enemies);

	/**
	 * The robot gets hit on a certain position
	 */
	void hit (int damage, core::vector3df position);

	void addXp(int xp);

	int getXp();

	void setLevelStart(bool levelStart);

	~Robot(void);

	/**
	 * The mesh of the robot, the robot itself
	 */
	EffActor* mesh;

	int health;

	bool isWeapon2Unlocked;

private:

	/**
	 * The function to call
	 */
	std::function<void(void)> onHit;

	/**
	 * Add gun
	 */
	void addGun(core::stringc gunMesh, core::vector3df position);

	void showWeapon2Unlock();

	/**
	 * Gun actor
	 */
	Gun* gun;

	/**
	 * URL XMLs of the weapon effects
	 */
	core::stringc shootEffectXML;
	core::stringc enemyHitEffectXML;
	core::stringc flyEffectXML;

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

	core::vector3df bulletOffset;

	/**
	 * Counter to keep track of the shoot cooldown
	 */
	float countShootCooldown;

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
	
	/**
	 * is level start
	 */
	bool levelStart;

	int xp;

	gui::IGUIImage* weapon2Unlock;
};

#endif

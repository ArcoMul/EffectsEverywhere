#ifndef BULLET_H_
#define BULLET_H_

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class Bullet : public EffActor
{

public:

	/**
	 * Default constructor
	 */
	Bullet (void);

	/**
	 * Constructor with position and rotation of the bullet
	 */
	Bullet (core::vector3df position, core::vector3df rotation);

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
	
	/**
	 * Temporary used to save the position and rotation until the node is made
	 * so we can set those
	 */
	core::vector3df startPosition;
	core::vector3df startRotation;
};

#endif

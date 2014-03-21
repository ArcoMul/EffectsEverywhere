#ifndef GUN_H_ 
#define GUN_H_ 

#include <irrlicht.h>
#include "EffActor.h"

using namespace irr;

class EffEngine;

class Gun : public EffActor
{
public:
	Gun (void);

	virtual void start (void);

	virtual void update (float deltaTime);

	/**
	 * When the robot shoots this function will be called
	 */
	void shoot (void);

	~Gun(void);

private:

	/**
	 * If the gun is 'shooting'
	 */
	bool shooting;

	/**
	 * In what direction the gun is moving
	 */
	int direction;
};

#endif

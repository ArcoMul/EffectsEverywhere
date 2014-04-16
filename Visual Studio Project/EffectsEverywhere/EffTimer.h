#ifndef EFFTIMER_H_
#define EFFTIMER_H_

#include <irrlicht.h>
#include <functional>
#include <map>

using namespace irr;

class EffScene;
class EffRepeatable;

class EffTimer
{
public:

	/**
	 * New timer class, timer needs the scene to get the current time of the game
	 */
	EffTimer(EffScene* scene);

	/**
	 * Tell the timer to repeatably call the given function every x seconds
	 */
	void repeat (std::function<void(void)>, float seconds);

	/**
	 * Update the timer, check if any functions need to be called
	 */
	void update (float deltaTime);

	/**
	 * Deconstructor
	 */
	~EffTimer(void);

private:

	/**
	 * List of function which need to be checked
	 */
	core::list<EffRepeatable*> repeatables;

	/**
	 * The scene this timer is part of
	 */
	EffScene* scene;

};

#endif
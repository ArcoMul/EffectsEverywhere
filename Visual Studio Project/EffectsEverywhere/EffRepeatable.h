#ifndef EFFREPEATABLE_H_
#define EFFREPEATABLE_H_

#include <functional>

class EffTimer;

class EffRepeatable
{
public:

	/**
	 * Create new repeatable 
	 * f - the function to call
	 * startTime - when this repeatable was created / called for the last time
	 * repeatAfter - call the functino of this repeatable every x seconds
	 */
	EffRepeatable(std::function<void(void)> f, EffTimer* timer, float startTime, float repeatAfter);

	/**
	 * Check if, and if so, call the function of this repeatable
	 */
	bool call (float time);

	void stop(void);

	/**
	 * Deconstructor
	 */
	~EffRepeatable(void);

private:

	/**
	 * The function to call
	 */
	std::function<void(void)> f;

	/**
	 * When this repeatable was created or when it was called the last time
	 */
	float startTime;

	/**
	 * Call this repeatable every x seconds
	 */
	float repeatAfter;

	EffTimer* timer;
};

#endif
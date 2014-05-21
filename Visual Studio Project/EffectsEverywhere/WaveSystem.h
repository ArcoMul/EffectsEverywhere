#ifndef WAVESYSTEM_H_ 
#define WAVESYSTEM_H_ 

#include "Wave.h"

class WaveSystem
{
public:
	WaveSystem(void);
	~WaveSystem(void);

	/**
	 * Add a new wave to this level's waves
	 */
	void addWave (Wave wave);

	/**
	 * Start executing the waves
	 */
	void start (void);

	/**
	 * Callback when the current wave is done
	 */
	void onWaveDone (void);

private:
	/**
	 * List with all the waves in this system
	 */
	core::list<Wave> waves;
};

#endif
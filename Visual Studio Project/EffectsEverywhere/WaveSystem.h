#ifndef WAVESYSTEM_H_ 
#define WAVESYSTEM_H_ 

#include "Wave.h"

class MainScene;

class WaveSystem
{
public:
	WaveSystem(MainScene* scene);
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

	/**
	 * Check if the next wave should start, or not yet
	 */
	void checkNextWave (void);

private:
	/**
	 * List with all the waves in this system
	 */
	core::list<Wave> waves;

	/**
	 * The total amount of waves at the start of the game
	 */
	int totalWaves;

	/**
	 * Reference to the scene, to spawn certain object and call functions
	 */
	MainScene* scene;

	/**
	 * If there is a wave spawning enemies
	 */
	bool isSpawning;
};

#endif
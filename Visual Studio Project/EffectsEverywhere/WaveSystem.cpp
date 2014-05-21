#include "WaveSystem.h"

WaveSystem::WaveSystem(void)
{
}

void WaveSystem::addWave (Wave wave)
{
	waves.push_back(wave);
}

void WaveSystem::start (void)
{
	// We dont start without any waves
	if (waves.size() == 0) return;

	// Bind a callback and start the wave
	(*waves.begin()).onDone = std::bind(&WaveSystem::onWaveDone, this);
	(*waves.begin()).start();
}

void WaveSystem::onWaveDone (void)
{
	// Remove the just finished wave
	waves.erase(waves.begin());

	// No more waves? You won the game!
	if (waves.size() == 0) {
		std::cout << "You won the game!" << std::endl;
		return;
	}

	// Start the next wave
	start();
}

WaveSystem::~WaveSystem(void)
{
}

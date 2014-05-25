#include "WaveSystem.h"
#include "MainScene.h"
#include "EndScene.h"

WaveSystem::WaveSystem(MainScene* scene)
{
	this->scene = scene;
	isSpawning = false;
}

void WaveSystem::addWave (Wave wave)
{
	waves.push_back(wave);
	totalWaves = waves.size();
}

void WaveSystem::start (void)
{
	// We dont start without any waves
	if (waves.size() == 0) return;

	isSpawning = true;

	scene->showWaveText(core::stringw("Wave ") + core::stringw(totalWaves - waves.size() + 1));

	// Bind a callback and start the wave
	(*waves.begin()).onDone = std::bind(&WaveSystem::onWaveDone, this);
	(*waves.begin()).start();
}

void WaveSystem::onWaveDone (void)
{
	// Remove the just finished wave
	waves.erase(waves.begin());

	isSpawning = false;

	// No more waves? You won the game!
	if (waves.size() == 0) {
		scene->switchScene(new EndScene(true,scene->getScore()));
		std::cout << "You won the game!" << std::endl;
		return;
	}

	checkNextWave();
}

void WaveSystem::checkNextWave (void)
{
	// If the wave is still spawning, or there are still enemies alive; dont start the next wave
	if (isSpawning || scene->enemiesAlive != 0) return;

	// Start the next wave
	start();
}

WaveSystem::~WaveSystem(void)
{
}

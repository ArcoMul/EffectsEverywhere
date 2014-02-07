#include <math.h>
#include "GameEngine.h"
#include "BackgroundFader.h"

GameEngine::GameEngine(IrrlichtDevice* device, IVideoDriver* driver)
{
	this->device = device;
	this->driver = driver;
	this->backgroundColor = SColor(255, 0, 0, 0);
	this->startTime = device->getTimer()->getTime();
	this->backgroundFader = new BackgroundFader(this);
}

void GameEngine::update (void)
{
	this->deltaTime = device->getTimer()->getTime() - this->lastFrameTime;
	this->totalTime = device->getTimer()->getTime() - this->startTime;

	backgroundFader->fade();

	//Begin drawing the scene, by deleting the Z buffer and clear the frame
	driver->beginScene(true, true, backgroundFader->getColor());

	//End drawing the scene
	driver->endScene();

	this->lastFrameTime = device->getTimer()->getTime();
}

GameEngine::~GameEngine(void)
{
	delete backgroundFader;
}

#include <math.h>
#include "GameEngine.h"
#include "BackgroundFader.h"

GameEngine::GameEngine()
{
	this->device = nullptr;
	this->driver = nullptr;
	this->backgroundColor = SColor(255, 0, 0, 0);
	this->startTime = 0;
	this->backgroundFader = new BackgroundFader(this);
}

bool GameEngine::init(int width, int height, int colordepth, bool fullscreen, bool stencilbuffer, bool vsyncenabled)
{
	//Create a device
	device = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(width, height), colordepth, 
										  fullscreen, false, vsyncenabled, 0);
	
	//Return if there is no device after the create function
	if (!device)
	{
		return false;
	}
	this->startTime = device->getTimer()->getTime();

	//Get the video driver from the device
	driver = device->getVideoDriver();


	if(!driver)
	{
		return false;
	}

	return true;
}

void GameEngine::run()
{
	//Game/Render loop
	while(device->run())
	{
		this->deltaTime = device->getTimer()->getTime() - this->lastFrameTime;
		this->totalTime = device->getTimer()->getTime() - this->startTime;

		update();
		draw();
		
		this->lastFrameTime = device->getTimer()->getTime();
	}
}

void GameEngine::update (void)
{
	
}

void GameEngine::draw (void)
{
	backgroundFader->fade();

	//Begin drawing the scene, by deleting the Z buffer and clear the frame
	driver->beginScene(true, true, backgroundFader->getColor());

	//End drawing the scene
	driver->endScene();
}

GameEngine::~GameEngine(void)
{
	delete device;
	delete backgroundFader;
}

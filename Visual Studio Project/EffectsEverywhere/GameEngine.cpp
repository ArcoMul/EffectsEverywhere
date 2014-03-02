#include <math.h>

#include "GameEngine.h"
#include "GameScene.h"
#include "InputReceiver.h"
#include "BackgroundFader.h"

GameEngine::GameEngine()
{
	this->device = nullptr;
	this->driver = nullptr;
	this->backgroundColor = SColor(255, 0, 0, 0);
	this->startTime = 0;
}

bool GameEngine::init(int width, int height, int colordepth, bool fullscreen, bool stencilbuffer, bool vsyncenabled)
{
	// Create the input event receiver
	inputReceiver = new InputReceiver;
	
	// Create a device
	device = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(width, height),
		colordepth, fullscreen, false, vsyncenabled, inputReceiver);

	// Return if there is no device after the create function
	if (!device)
	{
		return false;
	}

	// Get the start time of the engine
	this->startTime = device->getTimer()->getTime();

	// Get the video driver from the device
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	
	if(!driver)
	{
		return false;
	}

	return true;
}

void GameEngine::run()
{
	// Game/Render loop
	while(device->run())
	{
		this->deltaTime = device->getTimer()->getTime() - this->lastFrameTime;
		this->totalTime = device->getTimer()->getTime() - this->startTime;

		// Call all the update functions
		update();

		// Begin drawing the scene, by deleting the Z buffer and clear the frame
		driver->beginScene(true, true, backgroundColor);

		// Draw everything
		draw();
		
		// End drawing the scene
		driver->endScene();

		// Keep track of the time when leaving this frame, for use in the next frame
		this->lastFrameTime = device->getTimer()->getTime();
	}
}

void GameEngine::update (void)
{
	// Call the update function of the current scene
	activeScene->update();

	// Calculate delta mouse based on the previous mouse position and the current one
	deltaMouse = prevMouse - inputReceiver->cursor;

	// Save the current mouse position for the next frame
	prevMouse = inputReceiver->cursor;
}

void GameEngine::draw (void)
{
	// Draw all objects in the Irrlicht Scene Manager
	smgr->drawAll();
}

void GameEngine::setScene (GameScene* scene)
{
	// Keep track of the active scene
	activeScene = scene;

	// Initialize the new scene
	activeScene->start ();
}

GameEngine::~GameEngine(void)
{
	delete device;
}

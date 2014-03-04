#include <math.h>
#include <iostream>
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

	// Calculate screen size
	const irr::core::dimension2du& screenSize = device->getVideoDriver()->getScreenSize();
	// Lock mouse in screen
	if(!mouseLock)
		{
			// Check if the mouse in a box by 80 to 80 is in the center of the screen
		if (prevMouse.X <= (screenSize.Width/2)- 40 || prevMouse.X >= (screenSize.Width/2) + 40)
			{
			// Reset delta Mouse
			deltaMouse = position2di(0, 0);
			// Set Cursor in the middel of the screen
			device->getCursorControl()->setPosition(screenSize.Width/2, prevMouse.Y);
			}
		if (prevMouse.Y <= (screenSize.Height/2)- 40 || prevMouse.Y >= (screenSize.Height/2) + 40)
			{
			// Reset delta Mouse
			deltaMouse = position2di(0, 0);
			// Set Cursor in the middel of the screen
			device->getCursorControl()->setPosition(prevMouse.X,screenSize.Height/2);
			}
		}
	// Save the current mouse position for the next frame
	prevMouse = inputReceiver->cursor;
}

void GameEngine::setMouseVisible (bool mouseVisible)
{
	// Set mouse visible
	device->getCursorControl()->setVisible(mouseVisible);
	mouseLock = mouseVisible;
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

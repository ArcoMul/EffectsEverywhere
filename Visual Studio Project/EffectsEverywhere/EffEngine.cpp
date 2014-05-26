#include <math.h>
#include <iostream>
#include "EffEngine.h"
#include "EffScene.h"
#include "InputReceiver.h"
#include "BackgroundFader.h"

std::string EffEngine::basePath = "../../";

EffEngine::EffEngine()
{
	this->device = nullptr;
	this->driver = nullptr;
	this->backgroundColor = video::SColor(1, 0, 0, 0);
	this->startTime = 0;
	this->mouseLock = true;
}

std::string EffEngine::getPath (std::string path)
{
	return EffEngine::basePath.append(path);
}

bool EffEngine::init(int width, int height, int colordepth, bool fullscreen, bool stencilbuffer, bool vsyncenabled)
{
	// Create the input event receiver
	inputReceiver = new InputReceiver(this);
	if (!inputReceiver) return false;
	
	// Create a device
	device = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(width, height), colordepth, fullscreen, stencilbuffer, vsyncenabled, inputReceiver);
	if (!device) return false;

	// Get the video driver from the device
	driver = device->getVideoDriver();
	if (!driver) return false;

	smgr = device->getSceneManager();
	if (!smgr) return false;

	// create the gui
	gui = device->getGUIEnvironment();
	if(!gui) return false;

	// Get the start time of the engine
	this->startTime = device->getTimer()->getTime();
	this->lastFrameTime = device->getTimer()->getTime();
	
	return true;
}

void EffEngine::run()
{
	// Game/Render loop
	while(device->run())
	{
		float deltaTime = device->getTimer()->getTime() - this->lastFrameTime;

		// Call all the update functions
		update(deltaTime);

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

void EffEngine::update (float deltaTime)
{
	// Calculate delta mouse based on the previous mouse position and the current one
	deltaMouse = prevMouse - inputReceiver->cursor;

	if (mouseLock) lockMouse ();

	// Call the update function of the current scene
	activeEffScene->update(deltaTime);

	// Save the current mouse position for the next frame
	prevMouse = inputReceiver->cursor;
}

void EffEngine::lockMouse ()
{
	// Calculate screen size
	const irr::core::dimension2du& screenSize = device->getVideoDriver()->getScreenSize();

	// Check if the mouse in a box by 80 to 80 is in the center of the screen
	if (prevMouse.X <= (screenSize.Width/2)- 40 || prevMouse.X >= (screenSize.Width/2) + 40)
	{
		// Reset delta Mouse for this frame
		deltaMouse = position2di(0, 0);
		// Set Cursor in the middel of the screen
		device->getCursorControl()->setPosition(screenSize.Width/2, prevMouse.Y);
	}

	if (prevMouse.Y <= (screenSize.Height/2)- 40 || prevMouse.Y >= (screenSize.Height/2) + 40)
	{
		// Reset delta Mouse for this frame
		deltaMouse = position2di(0, 0);
		// Set Cursor in the middel of the screen
		device->getCursorControl()->setPosition(prevMouse.X,screenSize.Height/2);
	}
}

void EffEngine::setMouseVisible (bool mouseVisible)
{
	// Set mouse visible
	device->getCursorControl()->setVisible(mouseVisible);
	// Set mouse lock off / on
	mouseLock = !mouseVisible;
}

void EffEngine::onButtonClick(s32 id)
{
	activeEffScene->onButtonClick(id);
}

void EffEngine::draw (void)
{
	// Draw all objects in the Irrlicht Scene Manager
	smgr->drawAll();
	gui->drawAll();
}

void EffEngine::setScene (EffScene* scene)
{
	// Keep track of the active scene
	activeEffScene = scene;

	activeEffScene->setEngine (this);
	activeEffScene->setManager (smgr);
	activeEffScene->setGUI (gui);

	// Initialize the new scene
	activeEffScene->init ();
}

void EffEngine::switchScene (EffScene* scene)
{
	delete activeEffScene;
	smgr->clear();
	gui->clear();
    setScene (scene);
}

void EffEngine::closeGame (void)
{
	delete activeEffScene;
	smgr->clear();
	gui->clear();
	device->closeDevice();
}

EffEngine::~EffEngine(void)
{
	delete device;
}

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
	smgr = device->getSceneManager();

	// Temporary function to load some objects
	start();

	if(!driver)
	{
		return false;
	}

	return true;
}

void GameEngine::start()
{
	// The the mesh from the system
	IMesh* mesh = smgr->getMesh("../Media/robot.obj");

	// Add a new Irrlicht Node with the loaded mesh as mesh
	IMeshSceneNode* node = smgr->addMeshSceneNode(mesh);

	// Make sure the node is loaded and set what kind of matarial it is
	if (node) {
		node->setMaterialFlag(EMF_LIGHTING, false);
	}

	// Add the camera node to the scene
	smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));
}

void GameEngine::run()
{
	//Game/Render loop
	while(device->run())
	{
		this->deltaTime = device->getTimer()->getTime() - this->lastFrameTime;
		this->totalTime = device->getTimer()->getTime() - this->startTime;

		update();

		//Begin drawing the scene, by deleting the Z buffer and clear the frame
		driver->beginScene(true, true, backgroundFader->getColor());

		draw();
		
		//End drawing the scene
		driver->endScene();

		this->lastFrameTime = device->getTimer()->getTime();
	}
}

void GameEngine::update (void)
{
	backgroundFader->fade();
}

void GameEngine::draw (void)
{
	smgr->drawAll();
}

GameEngine::~GameEngine(void)
{
	delete device;
	delete backgroundFader;
}

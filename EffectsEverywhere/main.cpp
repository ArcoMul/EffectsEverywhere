
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

unsigned int screenWidth = 800;
unsigned int screenHeight = 600;
bool fullscreen = false;
bool vsyncEnabled = false; 

int main()
{
	//Create a device
	IrrlichtDevice *device = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(screenWidth, screenHeight), 16, 
										  fullscreen, false, vsyncEnabled, 0);
	
	//Return if there is no device after the create function
	if (!device)
	{
		return 1;
	}

	//Get the video driver from the device
	IVideoDriver* driver = device->getVideoDriver();

	//Game/Render loop
	while(device->run())
	{
		// Render stuff here
	}

	// Delete the Irrlicht engine/device
	delete device;

	return 0;
}
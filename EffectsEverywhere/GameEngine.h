#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class BackgroundFader;

class GameEngine
{
public:
	GameEngine();
	bool init(int width = 800, int height = 600, int colordepth = 16, bool fullscreen = false, 
		bool stencilbuffer = false, bool vsyncenabled = false); 
	void run();
	~GameEngine(void);

	float startTime;
	float totalTime;
	float lastFrameTime;
	float deltaTime;

private:	
	void update (void);
	void draw (void);

	IrrlichtDevice* device;
	IVideoDriver* driver;
	SColor backgroundColor;
	BackgroundFader* backgroundFader;
};

#endif

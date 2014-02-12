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
class GameScene;

class GameEngine
{
public:
	GameEngine();
	bool init(int width = 800, int height = 600, int colordepth = 16, bool fullscreen = false, 
		bool stencilbuffer = false, bool vsyncenabled = false); 
	void run();
	void setScene (GameScene* scene);
	~GameEngine(void);

	float startTime;
	float totalTime;
	float lastFrameTime;
	float deltaTime;

	ISceneManager* smgr;

	SColor backgroundColor;

private:	
	void update (void);
	void draw (void);

	IrrlichtDevice* device;
	IVideoDriver* driver;

	GameScene* activeScene;
};

#endif

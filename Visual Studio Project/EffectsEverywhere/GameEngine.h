#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class GameScene;
class InputReceiver;

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
	
	// Mouse position on the last frame
	position2di prevMouse;

	// The mouse change between this frame and last frame
	position2di deltaMouse;

	// Scene manager, keeps track of all nodes in the scene, I think (Arco)
	ISceneManager* smgr;

	// Controls input event receiver
	InputReceiver* inputReceiver;

	// Background color of the scene
	SColor backgroundColor;

private:	
	void update (void);
	void draw (void);

	IrrlichtDevice* device;
	IVideoDriver* driver;

	// Which scene we will call the update function of
	GameScene* activeScene;
};

#endif

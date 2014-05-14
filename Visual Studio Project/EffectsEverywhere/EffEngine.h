#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <irrlicht.h>
#include <ParticleManager.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EffScene;
class InputReceiver;

class EffEngine
{
public:
	EffEngine();
	bool init(int width = 800, int height = 600, int colordepth = 16, bool fullscreen = false, 
		bool stencilbuffer = false, bool vsyncenabled = false); 
	void run();
	void setScene (EffScene* scene);
	void switchScene (EffScene* scene);
	void setMouseVisible (bool mouseVisible);
	~EffEngine(void);
	
	// Mouse position on the last frame
	position2di prevMouse;

	// The mouse change between this frame and last frame
	position2di deltaMouse;

	// Scene manager, keeps track of all nodes in the scene, I think (Arco)
	ISceneManager* smgr;

	// Controls input event receiver
	InputReceiver* inputReceiver;

	IVideoDriver* driver;
	IrrlichtDevice* device;
	IGUIEnvironment* gui;

private:

	// Update all the objects in the game
	void update (float deltaTime);

	// Draws everything in the game
	void draw (void);

	// Lock the mouse in the screen
	void lockMouse (void);

	// Timestamp of the starttime
	float startTime;

	// Frame timestamp of the last frame
	float lastFrameTime;

	// If the mouse should be locked in the screen or not
	bool mouseLock;
	
	// Which scene we will call the update function of
	EffScene* activeEffScene;

	// Background color of the scene
	SColor backgroundColor;
};

#endif

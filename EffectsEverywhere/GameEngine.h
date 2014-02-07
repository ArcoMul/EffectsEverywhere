#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <irrlicht.h>

using namespace irr;
using namespace video;

class BackgroundFader;

class GameEngine
{
public:
	GameEngine(void);
	GameEngine(IrrlichtDevice*, IVideoDriver*);
	void update (void);
	~GameEngine(void);

	float startTime;
	float totalTime;
	float lastFrameTime;
	float deltaTime;
private:
	IrrlichtDevice* device;
	IVideoDriver* driver;
	SColor backgroundColor;
	BackgroundFader* backgroundFader;
};

#endif

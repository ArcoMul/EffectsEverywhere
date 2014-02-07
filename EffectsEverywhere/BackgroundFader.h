#ifndef BACKGROUNDFADER_H_
#define BACKGROUNDFADER_H_

#include <irrlicht.h>

using namespace irr;
using namespace video;

class GameEngine;

class BackgroundFader
{
public:
	BackgroundFader(GameEngine* engine);
	void fade(void);
	SColor getColor(void);
	~BackgroundFader(void);
private:
	GameEngine* _engine;
	SColor color;
	__int8 fadeDirection;
};

#endif 
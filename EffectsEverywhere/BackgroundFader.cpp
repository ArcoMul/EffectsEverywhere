#include <iostream>
#include "BackgroundFader.h"
#include "GameEngine.h"

BackgroundFader::BackgroundFader(GameEngine* engine) :
	_engine(engine), fadeDirection (-1)
{
	
}

void BackgroundFader::fade ()
{
	if (static_cast<int>(floor(_engine->totalTime)) % 10 == 0) {
		int newColor = color.getRed() + fadeDirection;
		color.setRed (newColor);
		color.setGreen (newColor);
		color.setBlue (newColor);
		if (newColor > 254) {
			fadeDirection *= -1;
		}
	}
}

SColor BackgroundFader::getColor ()
{
	return color;
}

BackgroundFader::~BackgroundFader(void)
{
	_engine = nullptr;
}

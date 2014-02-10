#include "GameEngine.h"

int main()
{
	//Create scene
	GameEngine* engine = new GameEngine();
	engine->init();

	engine->run();

	//Delete the engine
	delete engine;

	return 0;
}
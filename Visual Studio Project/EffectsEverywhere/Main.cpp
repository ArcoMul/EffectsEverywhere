#include "GameEngine.h"
#include "MainScene.h"

int main()
{
	// Create the engine
	GameEngine* engine = new GameEngine();

	// Start the engine
	engine->init();

	// Set the initial scene
	engine->setScene (new MainScene(engine));

	// Start running the engine
	engine->run();

	// Delete the engine
	delete engine;

	return 0;
}
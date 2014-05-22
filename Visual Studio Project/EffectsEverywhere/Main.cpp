#include <iostream>
#include "EffEngine.h"
#include "MainScene.h"
#include "StartScene.h"

int main()
{
	// Create the engine
	EffEngine* engine = new EffEngine();

	// Start the engine
	if (engine->init(1366, 768, 32, true, false, false))
	{
		// Set the initial scene
		engine->setScene (new StartScene());

		// Start running the engine
		engine->run();
	}
	else
	{
		std::cout << "Error starting EffEngine" << std::endl;
	}

	// Delete the engine after the game is finished
	delete engine;

	return 0;
}